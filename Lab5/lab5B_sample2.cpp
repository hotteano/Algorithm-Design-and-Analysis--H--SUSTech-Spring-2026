#include <iostream>
#include <limits>
#include <vector>

using namespace std;

using i64 = long long;

struct InputEdge {
    int from;
    int to;
    i64 cost;
};

struct WorkEdge {
    int from;
    int to;
    i64 cost;
    int prevEdgeIndex;
    int toInPrevLayer;
};

struct Layer {
    int nodeCount;
    int root;
    int cycleCount;
    vector<WorkEdge> edges;
    vector<int> inEdgeIndex;
    vector<i64> inCost;
    vector<int> contractedId;
    vector<vector<int>> cycles;
    vector<int> newNodeToOldVertex;
};

struct ArborescenceResult {
    bool feasible;
    i64 totalCost;
    vector<int> parent;
};

ArborescenceResult directedMST(int nodeCount, int root, const vector<InputEdge>& inputEdges) {
    const i64 INF = numeric_limits<i64>::max() / 4;

    vector<Layer> layers;
    vector<WorkEdge> currentEdges;
    currentEdges.reserve(inputEdges.size());
    for (const auto& edge : inputEdges) {
        currentEdges.push_back({edge.from, edge.to, edge.cost, -1, edge.to});
    }

    int currentNodeCount = nodeCount;
    int currentRoot = root;
    i64 totalCost = 0;

    while (true) {
        Layer layer;
        layer.nodeCount = currentNodeCount;
        layer.root = currentRoot;
        layer.edges = currentEdges;
        layer.inEdgeIndex.assign(currentNodeCount, -1);
        layer.inCost.assign(currentNodeCount, INF);

        for (int edgeIndex = 0; edgeIndex < static_cast<int>(currentEdges.size()); ++edgeIndex) {
            const auto& edge = currentEdges[edgeIndex];
            if (edge.from == edge.to) {
                continue;
            }
            if (edge.cost < layer.inCost[edge.to]) {
                layer.inCost[edge.to] = edge.cost;
                layer.inEdgeIndex[edge.to] = edgeIndex;
            }
        }

        for (int vertex = 0; vertex < currentNodeCount; ++vertex) {
            if (vertex == currentRoot) {
                continue;
            }
            if (layer.inEdgeIndex[vertex] == -1) {
                return {false, 0, {}};
            }
            totalCost += layer.inCost[vertex];
        }

        vector<int> visited(currentNodeCount, -1);
        layer.contractedId.assign(currentNodeCount, -1);
        layer.cycleCount = 0;

        for (int start = 0; start < currentNodeCount; ++start) {
            int vertex = start;
            while (visited[vertex] != start && layer.contractedId[vertex] == -1 && vertex != currentRoot) {
                visited[vertex] = start;
                vertex = currentEdges[layer.inEdgeIndex[vertex]].from;
            }
            if (vertex != currentRoot && layer.contractedId[vertex] == -1) {
                vector<int> cycleVertices;
                cycleVertices.push_back(vertex);
                for (int u = currentEdges[layer.inEdgeIndex[vertex]].from; u != vertex;
                     u = currentEdges[layer.inEdgeIndex[u]].from) {
                    cycleVertices.push_back(u);
                }
                for (int u : cycleVertices) {
                    layer.contractedId[u] = layer.cycleCount;
                }
                layer.cycles.push_back(cycleVertices);
                ++layer.cycleCount;
            }
        }

        if (layer.cycleCount == 0) {
            layers.push_back(std::move(layer));
            break;
        }

        int newNodeCount = layer.cycleCount;
        layer.newNodeToOldVertex.assign(currentNodeCount, -1);
        for (int vertex = 0; vertex < currentNodeCount; ++vertex) {
            if (layer.contractedId[vertex] == -1) {
                layer.contractedId[vertex] = newNodeCount;
                layer.newNodeToOldVertex[newNodeCount] = vertex;
                ++newNodeCount;
            }
        }

        vector<WorkEdge> nextEdges;
        nextEdges.reserve(currentEdges.size());
        for (int edgeIndex = 0; edgeIndex < static_cast<int>(currentEdges.size()); ++edgeIndex) {
            const auto& edge = currentEdges[edgeIndex];
            int newFrom = layer.contractedId[edge.from];
            int newTo = layer.contractedId[edge.to];
            if (newFrom == newTo) {
                continue;
            }
            nextEdges.push_back(
                {newFrom, newTo, edge.cost - layer.inCost[edge.to], edgeIndex, edge.to});
        }

        int nextRoot = layer.contractedId[currentRoot];
        layers.push_back(std::move(layer));
        currentEdges = std::move(nextEdges);
        currentNodeCount = newNodeCount;
        currentRoot = nextRoot;
    }

    int lastLayerIndex = static_cast<int>(layers.size()) - 1;
    vector<vector<int>> chosenEdgeIndex(layers.size());
    chosenEdgeIndex[lastLayerIndex].assign(layers[lastLayerIndex].nodeCount, -1);
    for (int vertex = 0; vertex < layers[lastLayerIndex].nodeCount; ++vertex) {
        if (vertex == layers[lastLayerIndex].root) {
            continue;
        }
        chosenEdgeIndex[lastLayerIndex][vertex] = layers[lastLayerIndex].inEdgeIndex[vertex];
    }

    for (int layerIndex = lastLayerIndex - 1; layerIndex >= 0; --layerIndex) {
        const auto& layer = layers[layerIndex];
        const auto& nextLayer = layers[layerIndex + 1];

        vector<int> selected(layer.nodeCount, -1);
        vector<int> cycleEnteringEdge(layer.cycleCount, -1);
        vector<int> cycleEnteringTarget(layer.cycleCount, -1);

        for (int newNode = 0; newNode < nextLayer.nodeCount; ++newNode) {
            if (newNode == nextLayer.root) {
                continue;
            }
            int edgeInNext = chosenEdgeIndex[layerIndex + 1][newNode];
            if (edgeInNext < 0 || edgeInNext >= static_cast<int>(nextLayer.edges.size())) {
                return {false, 0, {}};
            }
            const auto& selectedNextEdge = nextLayer.edges[edgeInNext];
            int mappedEdgeInCurrent = selectedNextEdge.prevEdgeIndex;
            int toVertexInCurrent = selectedNextEdge.toInPrevLayer;

            if (newNode < layer.cycleCount) {
                cycleEnteringEdge[newNode] = mappedEdgeInCurrent;
                cycleEnteringTarget[newNode] = toVertexInCurrent;
            } else {
                int oldVertex = layer.newNodeToOldVertex[newNode];
                if (oldVertex < 0 || oldVertex >= layer.nodeCount) {
                    return {false, 0, {}};
                }
                selected[oldVertex] = mappedEdgeInCurrent;
            }
        }

        for (int cycleId = 0; cycleId < layer.cycleCount; ++cycleId) {
            for (int vertex : layer.cycles[cycleId]) {
                selected[vertex] = layer.inEdgeIndex[vertex];
            }
            if (cycleEnteringTarget[cycleId] < 0 || cycleEnteringTarget[cycleId] >= layer.nodeCount) {
                return {false, 0, {}};
            }
            selected[cycleEnteringTarget[cycleId]] = cycleEnteringEdge[cycleId];
        }

        chosenEdgeIndex[layerIndex] = std::move(selected);
    }

    vector<int> parent(nodeCount, -1);
    const auto& firstLayer = layers.front();
    for (int vertex = 0; vertex < nodeCount; ++vertex) {
        if (vertex == root) {
            continue;
        }
        int edgeIndex = chosenEdgeIndex[0][vertex];
        if (edgeIndex < 0 || edgeIndex >= static_cast<int>(firstLayer.edges.size())) {
            return {false, 0, {}};
        }
        parent[vertex] = firstLayer.edges[edgeIndex].from;
    }

    return {true, totalCost, parent};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n >> m) {
        if (n <= 0 || m < 0) {
            cout << "impossible\n\n";
            continue;
        }

        vector<InputEdge> edges;
        edges.reserve(m + n);
        i64 sumCost = 0;
        bool hasInvalidEndpoint = false;

        for (int i = 0; i < m; ++i) {
            int from, to;
            i64 cost;
            cin >> from >> to >> cost;

            if (from < 0 || from >= n || to < 0 || to >= n) {
                hasInvalidEndpoint = true;
                continue;
            }

            ++from;
            ++to;
            if (from == to) {
                continue;
            }
            edges.push_back({from, to, cost});
            sumCost += cost;
        }

        if (hasInvalidEndpoint) {
            cout << "impossible\n\n";
            continue;
        }

        const i64 superRootEdgeCost = sumCost + 1;
        for (int node = 1; node <= n; ++node) {
            edges.push_back({0, node, superRootEdgeCost});
        }

        ArborescenceResult result = directedMST(n + 1, 0, edges);
        if (!result.feasible || result.totalCost > 2 * sumCost + 1) {
            cout << "impossible\n\n";
            continue;
        }

        int chosenRoot = -1;
        for (int node = 1; node <= n; ++node) {
            if (result.parent[node] == 0) {
                chosenRoot = node;
                break;
            }
        }
        if (chosenRoot == -1) {
            cout << "impossible\n\n";
            continue;
        }

        cout << (result.totalCost - superRootEdgeCost) << ' ' << (chosenRoot - 1) << '\n';

        for (int node = 1; node <= n; ++node) {
            int originalNode = node - 1;
            if (node == chosenRoot) {
                cout << originalNode << '\n';
            } else {
                cout << originalNode << ' ' << (result.parent[node] - 1) << '\n';
            }
        }
        cout << '\n';
    }

    return 0;
}