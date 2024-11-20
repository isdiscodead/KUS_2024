# 배열을 주어진 기준 값(pivot)을 기준으로 분할하는 함수
def partition(arr, low, high, pivot):
    # 기준 값(pivot)의 인덱스를 찾음
    pivot_index = arr.index(pivot)
    # 기준 값을 배열의 마지막 요소와 교환
    arr[pivot_index], arr[high] = arr[high], arr[pivot_index]
    
    i = low
    # 배열을 순회하면서 기준 값보다 작은 값들은 왼쪽으로 이동
    for j in range(low, high):
        if arr[j] < pivot:
            arr[i], arr[j] = arr[j], arr[i]
            i += 1
    # 기준 값을 올바른 위치에 배치
    arr[i], arr[high] = arr[high], arr[i]
    return i

# 중간값의 중간값을 찾는 함수
def median_of_medians(arr, low, high):
    # 배열의 크기가 5 이하일 경우, 배열을 정렬한 후 중간값 반환
    if high - low < 5:
        return sorted(arr[low:high+1])[(high - low) // 2]
    
    # 배열을 5개의 원소로 이루어진 그룹으로 나누고 각 그룹의 중간값을 찾음
    medians = []
    i = low
    while i <= high:
        group = arr[i:min(i + 5, high + 1)]
        medians.append(sorted(group)[len(group) // 2])
        i += 5
    
    # 재귀적으로 중간값들의 중간값을 찾음
    return median_of_medians(medians, 0, len(medians) - 1)

# Median of Medians 알고리즘을 사용하여 k번째 작은 원소를 찾는 함수
def select(arr, low, high, k):
    # 기저 사례: 배열에 원소가 하나밖에 없을 경우 그 원소를 반환
    if low == high:
        return arr[low]
    
    # 중간값의 중간값을 찾아 기준 값(pivot)으로 사용
    pivot = median_of_medians(arr, low, high)
    
    # 배열을 기준 값(pivot)으로 분할
    pivot_index = partition(arr, low, high, pivot)
    
    # 기준 값의 순위를 계산
    rank = pivot_index - low + 1
    
    # k번째 원소가 기준 값일 경우, 해당 값을 반환
    if rank == k:
        return arr[pivot_index]
    # k번째 원소가 왼쪽 부분에 있을 경우, 왼쪽 부분에서 재귀적으로 탐색
    elif k < rank:
        return select(arr, low, pivot_index - 1, k)
    # k번째 원소가 오른쪽 부분에 있을 경우, 오른쪽 부분에서 재귀적으로 탐색
    else:
        return select(arr, pivot_index + 1, high, k - rank)

# k번째 작은 원소를 찾는 메인 함수
def kth_smallest(arr, k):
    # 입력 값 유효성 검사
    if k < 1 or k > len(arr):
        raise ValueError("k의 값이 배열 크기를 벗어났습니다.")
    return select(arr, 0, len(arr) - 1, k)

# 사용 예시
arr = [12, 3, 5, 7, 4, 19, 26]
k = 3
result = kth_smallest(arr, k)
print(f"배열에서 {k}번째 작은 원소는 {result}입니다.")