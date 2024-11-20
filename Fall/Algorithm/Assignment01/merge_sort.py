def merge_sort(A, left, right):  # sort A[left..right] ascending order
    if left < right:
        mid = (left + right) // 2
        merge_sort(A, left, mid)
        merge_sort(A, mid + 1, right)
        merge(A, left, mid, right)  
    return A

def merge(A, left, mid, right):
    # mid 인덱스를 기준으로 양쪽으로 분할
    L = A[left:mid+1]   # A[left..mid]
    R = A[mid+1:right+1]  # A[mid+1..right]

    i = 0
    j = 0
    k = left

    # L과 R을 비교하면서 병합
    while i < len(L) and j < len(R):
        if L[i] <= R[j]:
            A[k] = L[i]
            i += 1
        else:
            A[k] = R[j]
            j += 1
        k += 1

    # L 배열에 남은 요소가 있으면 병합
    while i < len(L):
        A[k] = L[i]
        i += 1
        k += 1

    # R 배열에 남은 요소가 있으면 병합
    while j < len(R):
        A[k] = R[j]
        j += 1
        k += 1

# 테스트 코드
tmp = [9, 2, 1, 3, 8, 12]
print(merge_sort(tmp, 0, len(tmp)-1))
