#define _PROCESS_H_
#define _PSTREE_H_
#define _CRT_SECURE_NO_WARNINGS


#include <iostream>

// to get num of running processes
#include <wtypes.h> 
#include <Psapi.h> 

#include <TlHelp32.h> // to use createToolhelp32Snapshot()
#include <vector>

#include <tchar.h> // to use TCHAR
#include <wtypes.h> // to use DWORD


// Process Ŭ���� ����
class Process {
	TCHAR* name; // ���μ����� �̸�
	DWORD pid, ppid; // ���μ��� id�� parent ���μ��� id ��ȣ
	DWORD newPpid; // parent ���μ����� ���� ��� ���� parent process�� �Ǵ� root ���μ���
	std::vector<Process> children; // child ���μ��� ���

public:
	Process(TCHAR name[], DWORD pid, DWORD ppid) {
		int nameLength = _tcslen(name);
		this->name = new TCHAR[nameLength + 1];
		wcscpy(this->name, name); // �̸� ����
		this->pid = pid;
		this->ppid = ppid;
		newPpid = ppid;
	}

	/// get methods
	Process* getChild(int i) { return &children[i]; };
	int getSizeOfChildren() { return children.size(); };
	DWORD getPID() { return pid; };
	DWORD getPPID() { return ppid; };

	// set methods
	void setNewPPID(DWORD newPpid) { this->newPpid = newPpid; };
	void addChild(Process p) { children.push_back(p); };

	// child ���μ��� ���� ����
	bool hasChild() {
		if (children.size()) return true;
		return false;
	};

	// ���μ��� ����(�̸�, pid, root pid, ppid ) ���
	void printProcInfo() {
		printf("+-%S(%d) (%d:%d)\n", name, pid, newPpid, ppid);
	};
};


// PSTree Ŭ���� ����
class PSTree {
	Process* root; // root process ( init process )

public:
	// ������ �Լ�
	// root process �ʱ�ȭ
	PSTree(TCHAR nameOfRoot[], DWORD pidOfRoot, DWORD ppidOfRoot) {
		root = new Process(nameOfRoot, pidOfRoot, ppidOfRoot);
	}

	// parent ���μ����� ã�� �Լ�
	// input : current process�� �ּ�, ppid
	// output : �θ� ���μ����� �ּ�
	//		    �θ� ���μ����� �׾� ã�� �� ���� ���, return null
	Process* findParentProcess(Process* currentProcess, DWORD ppid) {
		// ���� ���μ����� parent process�� ��� return address of current process
		if (currentProcess->getPID() == ppid) {
			return currentProcess;
		}

		// child�� �ִ� ���
		if (currentProcess->hasChild()) {
			Process* child = NULL;
			Process* parent = NULL;

			// current ���μ����� child�� ��ȸ�ϸ� parent process ã��
			for (std::vector<Process>::size_type iOfChild = 0; iOfChild < currentProcess->getSizeOfChildren(); iOfChild++) {
				child = currentProcess->getChild(iOfChild);
				if (parent = findParentProcess(child, ppid)) // ã���� return
					return parent;
			}

			// ã�� �� ���� ��� return null
			return NULL;

		} else { 
			return NULL;
		}
	}
	;

	// ���μ��� Ʈ���� ���μ��� �߰��ϴ� �Լ�
	// input : new process�� ����
	void addProc(TCHAR name[], DWORD pid, DWORD ppid) {
		Process newProc(name, pid, ppid);

		// new ���μ����� parent ã��
		Process* parent = findParentProcess(root, newProc.getPPID());

		// ã�� �� ���� ���, ��Ʈ ���μ����� ����
		if (parent == NULL) {
			newProc.setNewPPID(0);
			root->addChild(newProc);

		} else { // ã�� ��� parent�� �߰�
			newProc.setNewPPID(parent->getPID());
			parent->addChild(newProc);
		}
	}
	;

	// input : current ���μ��� �ּ�, ����� ���� indentations
	// output : ���μ��� ���� ���
	void printProcess(Process* currentProc, int indent, int* iOfProc) {
		// child�� ���� ��� ��ȸ�ϸ� ���
		Process* child;
		for (std::vector<Process>::size_type iOfChild = 0; iOfChild < currentProc->getSizeOfChildren(); iOfChild++) {
			printf("%03d ", (*iOfProc)++);
			for (int j = 0; j <= indent; j++) {
				printf("|\t");
			}
			child = currentProc->getChild(iOfChild);
			child->printProcInfo();

			// child�� child�� ��� ... 
			if (child->hasChild()) {
				printProcess(child, indent + 1, iOfProc);
			}
		}
		indent++;
	};

	// process tree ���
	void printPSTree() {
		int iOfProc = 1; // ��µǴ� ���μ������� index

		// ��Ʈ ���μ��� ���� ���� ���
		printf("%03d ", iOfProc++);
		root->printProcInfo();

		// ������ ���μ��� ���
		printProcess(root, 0, &iOfProc);
	};
};


// main
int main() {

	// identifier ����
	HANDLE hProcessSnap; // ���μ����� snap
	PROCESSENTRY32 pe32; // ���μ����� ������ ��� ����ü
	int numOfProc = 0; // �� ���μ����� ��

	// snap process
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		puts("CreateToolhelp32Snapchot error");
		exit(EXIT_FAILURE);
	}

	// init ���μ��� ���� ��������
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32)) {
		puts("Process32First error!");
		CloseHandle(hProcessSnap);
		exit(EXIT_FAILURE);
	}
	numOfProc++; // ���μ��� �� 1 ����

	// ��� ���μ��� ��������
	PSTree pstree(pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID); // init ���μ��� = root

	// ���� ���μ������� ������ �޾� Ʈ�� �����
	while (Process32Next(hProcessSnap, &pe32)) {
		pstree.addProc(pe32.szExeFile, pe32.th32ProcessID, pe32.th32ParentProcessID);
		numOfProc++;
	}

	// ��� ������ ���� ���
	puts(" ########### process tree ############");
	printf("Number of Running Processes = %d\n", numOfProc);
	pstree.printPSTree();

}