import random, subprocess, sys

def run_et(n,m,r,edges):
    s=[f"{n} {m} {r}"]+[f"{u} {v} {w}" for u,v,w in edges]
    p=subprocess.run(["./Edmonds_Tarjan.exe"],input="\n".join(s)+"\n",text=True,capture_output=True)
    return p.stdout.strip()

def run_cl(n,m,edges):
    s=[f"{n} {m}"]+[f"{u} {v} {w}" for u,v,w in edges]
    p=subprocess.run(["./Chu_Liu.exe"],input="\n".join(s)+"\n",text=True,capture_output=True)
    return p.stdout.strip().split()[0] if p.stdout.strip() else ""

random.seed(0)
for n in range(2,9):
  r=0
  for _ in range(20000):
    m=random.randint(n-1,n*(n-1))
    es=[]
    for __ in range(m):
      u=random.randrange(n); v=random.randrange(n)
      w=random.randint(-5,20)
      es.append((u,v,w))
    a=run_et(n,m,r,es)
    b=run_cl(n,m,es)
    if a!=b:
      # interested in ET impossible while CL has value
      if a=="impossible" and b!="impossible":
        print("FOUND",n,m,r)
        print(n,m,r)
        for e in es: print(*e)
        print("ET",a,"CL",b)
        sys.exit(0)
print("no mismatch found")
