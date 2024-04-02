import numpy as np 

def locateLeftTurner(n, l):
    veh = np.random.rand(n) # 0~1 사이의 균일 분포
    index = np.argsort(veh)
    
    leftTurners = index[:l] # 첫 l 대의 차량
    
    return leftTurners


def testABC1(leftTurners, n, l):
    if len(leftTurners) < 4:
        # 조건을 충족하는 차량이 충분하지 않을 때
        return [False, False, False]
    else:
        # 조건에 따라 결과 계산
        condition1 = leftTurners[3] == 4 if 4 in leftTurners else False
        condition2 = all(x > 4 for x in leftTurners) if len(leftTurners) > 0 else False
        condition3 = (leftTurners[-1] == n-1) and (leftTurners[-2] == n-2) if len(leftTurners) >= 2 else False
        return [condition1, condition2, condition3]


def testABC(leftTurners, n, l):
    if len(leftTurners) < 4:
        # 조건을 충족하는 차량이 충분하지 않을 때
        return [False, False, False]
    else:
        # 조건에 따라 결과 계산
        vtestResult = [(leftTurners[l-1] == l), sum(leftTurners<=4)==0, ((leftTurners[l-1] == n-1) & (leftTurners[l-2] == n-2))] 

    return(vtestResult)


def simulation(n, l, m):
    vProbABC = np.zeros((3,1))
    result = np.zeros((3,m))
    for i in range(m):
        result[:,i] = testABC(locateLeftTurner(n, l), n, l)
    vProbABC[0] = np.mean(result[0])
    vProbABC[1] = np.mean(result[1])
    vProbABC[2] = np.mean(result[2])
    return vProbABC