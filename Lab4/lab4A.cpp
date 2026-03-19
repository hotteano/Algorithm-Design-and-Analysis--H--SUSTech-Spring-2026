#include <bits/stdc++.h>
using namespace std;

struct Node {
	int key;
	int value;
	int freq;
	long long lastUsed;
	long long expireTime;
	// 记录当前节点在“对应频次链表”中的位置，便于 O(1) 删除和移动。
	list<Node*>::iterator freqIt;
	// 记录当前节点在“全局更新时间链表”中的位置，便于维护最终输出顺序。
	list<Node*>::iterator recIt;
};

class LFUCache {
public:
	LFUCache(int capacity, long long ttl)
		: cap(capacity), T(ttl), minFreq(0), now(0) {}

	int get(int key) {
		// 题目要求：每次操作前都必须先清理所有过期键。
		expireBeforeOp();
		int ans = -1;
		auto it = keyToNode.find(key);
		if (it != keyToNode.end()) {
			Node* node = it->second;
			ans = node->value;
			// get 命中视为一次“使用”：频次 +1，更新最近使用顺序，并刷新过期时间。
			touch(node);
		}
		// 每条操作消耗 1 个时间单位。
		++now;
		return ans;
	}

	void put(int key, int value) {
		// 同样先做过期清理，保证操作开始时缓存内不存在超时键。
		expireBeforeOp();
		if (cap == 0) {
			// 容量为 0 时无法存储任何键，但时间仍会流逝。
			++now;
			return;
		}

		auto it = keyToNode.find(key);
		if (it != keyToNode.end()) {
			Node* node = it->second;
			node->value = value;
			// put 命中同样算一次使用，频次和时效都要刷新。
			touch(node);
			++now;
			return;
		}

		if ((int)keyToNode.size() >= cap) {
			// 满容量时先淘汰：优先 LFU；同频次下淘汰最久未使用（LRU）。
			evictLFU();
		}

		Node* node = createNode(key, value);

		keyToNode[key] = node;
		auto& lst = freqToList[1];
		lst.push_back(node);
		node->freqIt = prev(lst.end());

		recency.push_back(node);
		node->recIt = prev(recency.end());

		minFreq = 1;
		// 新插入节点需要登记未来过期时刻。
		scheduleExpire(node);
		++now;
	}

	vector<int> finalValuesByUpdateTime() const {
		// 按全局更新时间顺序输出（头最旧，尾最新）。
		vector<int> vals;
		vals.reserve(recency.size());
		for (Node* node : recency) {
			vals.push_back(node->value);
		}
		return vals;
	}

private:
	int cap;
	long long T;
	int minFreq;
	long long now;

	unordered_map<int, Node*> keyToNode;
	// 频次 -> 节点链表：每个频次内部用 LRU 顺序维护（头旧尾新）。
	unordered_map<int, list<Node*>> freqToList;
	// 全局更新时间链表：用于题目最后一行输出顺序。
	list<Node*> recency;
	// 过期时间桶：key 为时刻，value 为该时刻需要检查的候选节点。
	unordered_map<long long, vector<Node*>> expireBuckets;
	// 统一托管节点内存，保证 Node* 在容器中长期稳定可用。
	vector<unique_ptr<Node>> pool;

	Node* createNode(int key, int value) {
		auto holder = make_unique<Node>();
		Node* node = holder.get();
		pool.push_back(move(holder));

		node->key = key;
		node->value = value;
		node->freq = 1;
		node->lastUsed = now;
		// 过期定义：超过 T 个时间单位未使用即过期，因此设为 now + T + 1。
		node->expireTime = now + T + 1;
		return node;
	}

	void scheduleExpire(Node* node) {
		// 把节点登记到其过期时刻对应的桶中。
		expireBuckets[node->expireTime].push_back(node);
	}

	void expireBeforeOp() {
		// 只处理当前时刻 now 的桶，避免每次扫描全表。
		auto it = expireBuckets.find(now);
		if (it == expireBuckets.end()) {
			return;
		}

		for (Node* node : it->second) {
			auto live = keyToNode.find(node->key);
			// 节点可能被多次 touch，旧桶里会残留历史记录。
			// 因此必须校验：
			// 1) 该 key 还在缓存里；
			// 2) map 中指针仍指向该节点（不是旧节点）；
			// 3) 过期时刻确实到达。
			if (live != keyToNode.end() && live->second == node && node->expireTime <= now) {
				removeNode(node);
			}
		}
		// 当前时刻处理完后直接删除桶，避免重复处理。
		expireBuckets.erase(it);
	}

	void touch(Node* node) {
		// touch 统一封装“使用一次”后的全部状态迁移。
		int oldFreq = node->freq;

		// 1) 从旧频次链表移除。
		auto itOld = freqToList.find(oldFreq);
		itOld->second.erase(node->freqIt);
		if (itOld->second.empty()) {
			// 旧频次层空了就删掉该层。
			freqToList.erase(itOld);
			if (minFreq == oldFreq) {
				// 若最小频次层被清空，最小频次需要向上移动。
				++minFreq;
			}
		}

		// 2) 频次 +1，加入新频次层的尾部（最新）。
		node->freq = oldFreq + 1;
		auto& newList = freqToList[node->freq];
		newList.push_back(node);
		node->freqIt = prev(newList.end());

		// 3) 在全局更新时间链表中移动到尾部。
		recency.erase(node->recIt);
		recency.push_back(node);
		node->recIt = prev(recency.end());

		// 4) 刷新使用时刻与过期时刻，并重新登记过期桶。
		node->lastUsed = now;
		// “超过 T 时间未使用” 等价为在 lastUsed + T + 1 时开始视为过期。
		node->expireTime = now + T + 1;
		scheduleExpire(node);
	}

	void removeNode(Node* node) {
		// removeNode 统一处理删除动作：可被过期清理和 LFU 淘汰复用。
		int f = node->freq;

		// 1) 从频次层删除。
		auto fit = freqToList.find(f);
		if (fit != freqToList.end()) {
			fit->second.erase(node->freqIt);
			if (fit->second.empty()) {
				freqToList.erase(fit);
			}
		}

		// 2) 从全局更新时间链表删除。
		recency.erase(node->recIt);
		// 3) 从 key 索引删除。
		keyToNode.erase(node->key);

		if (keyToNode.empty()) {
			// 缓存被清空，minFreq 重置为 0。
			minFreq = 0;
			return;
		}

		if (f == minFreq && freqToList.find(minFreq) == freqToList.end()) {
			// 若删除的是当前最小频次层，且该层已空，向上找到下一个存在的频次层。
			while (freqToList.find(minFreq) == freqToList.end()) {
				++minFreq;
			}
		}
	}

	void evictLFU() {
		if (keyToNode.empty()) {
			return;
		}

		// 防御性修正：确保 minFreq 指向一个真实存在且非空的频次层。
		while (freqToList.find(minFreq) == freqToList.end() || freqToList[minFreq].empty()) {
			++minFreq;
		}

		// 在最小频次层里，链表头是最久未使用节点（同频次下的 LRU）。
		Node* victim = freqToList[minFreq].front();
		removeNode(victim);
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N, M;
	long long T;
	// 输入失败直接退出。
	if (!(cin >> N >> M >> T)) {
		return 0;
	}

	LFUCache cache(N, T);
	vector<int> outputs;
	outputs.reserve(M);

	for (int i = 0; i < M; ++i) {
		int op;
		cin >> op;
		if (op == 1) {
			// get: 输出值或 -1。
			int k;
			cin >> k;
			outputs.push_back(cache.get(k));
		} else {
			// put: 插入或更新。
			int k, v;
			cin >> k >> v;
			cache.put(k, v);
		}
	}

	// 先输出所有 get 的结果。
	for (int x : outputs) {
		cout << x << '\n';
	}

	// 再输出最终缓存中的 value，顺序为“最久未更新 -> 最近更新”。
	vector<int> finalVals = cache.finalValuesByUpdateTime();
	if (finalVals.empty()) {
		cout << "empty\n";
	} else {
		for (int i = 0; i < (int)finalVals.size(); ++i) {
			if (i) cout << ' ';
			cout << finalVals[i];
		}
		cout << '\n';
	}

	return 0;
}
