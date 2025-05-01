#include <iostream>
#include <cstdlib>

#define MAX 20

int pStatus[MAX], n, coordinator;
using namespace std;

void bully();
void ring();
void display();

int main() {
    int i, j, fchoice;
    cout << "Enter number of processes: ";
    cin >> n;
    for (i = 1; i <= n; i++) {
        cout << "Enter Process " << i << " is alive or not(0/1): ";
        cin >> pStatus[i];
        if (pStatus[i]) coordinator = i;
    }
    display();

    do {
        cout << "\n1.BULLY ALGORITHM\n2.RING\n3.DISPLAY\n4.EXIT\n";
        cout << "Enter your choice: ";
        cin >> fchoice;
        switch (fchoice) {
            case 1:
                bully();
                break;
            case 2:
                ring();
                break;
            case 3:
                display();
                break;
            case 4:
                exit(1);
                break;
        }
    } while (fchoice != 4);
    return 0;
}

void display() {
    int i;
    cout << "\nProcesses: ";
    for (i = 1; i <= n; i++) 
        cout << i << "\t";
    cout << endl << "Alive: ";
    for (i = 1; i <= n; i++)
        cout << pStatus[i] << "\t";
    cout << "\nCOORDINATOR IS " << coordinator << endl;
}

void bully() {
    int schoice, crash, activate, i, gid, flag, subcoordinator;
    do {
        cout << "\n1.CRASH\n2.ACTIVATE\n3.DISPLAY\n4.EXIT\n";
        cout << "Enter your choice: ";
        cin >> schoice;
        switch (schoice) {
            case 1:
                cout << "Enter process to crash: ";
                cin >> crash;
                if (pStatus[crash]) {
                    pStatus[crash] = 0;
                } else {
                    cout << "Process " << crash << " is already dead!" << endl;
                }
                do {
                    cout << "Enter election generator id: ";
                    cin >> gid;
                    if (gid == coordinator || pStatus[gid] == 0) {
                        cout << "Please, enter a valid generator id.." << endl;
                    }
                    
                } while (gid == coordinator || pStatus[gid] == 0);
                flag = 0;
                if (crash == coordinator) {
                    for (i = gid + 1; i <= n; i++) {
                        cout << "Message is sent from " << gid << " to " << i << endl;
                        if (pStatus[i]) {
                            subcoordinator = i;
                            cout << "Response is sent from " << i << " to " << gid << endl;
                            flag = 1;
                        }
                    }
                    if (flag == 1) {
                        coordinator = subcoordinator;
                    } else {
                        coordinator = gid;
                    }
                }
                display();
                break;
            case 2:
                cout << "Enter Process ID to be activated: ";
                cin >> activate;
                if (!pStatus[activate]) {
                    pStatus[activate] = 1;
                } else {
                    cout << "Process " << activate << " is already alive!" << endl;
                    break;
                }
                if (activate == n) {
                    coordinator = n;
                    break;
                }
                flag = 0;
                for (i = activate + 1; i <= n; i++) {
                    cout << "Message is sent from " << activate << " to " << i << endl;
                    if (pStatus[i]) {
                        subcoordinator = i;
                        cout << "Response is sent from " << i << " to " << activate << endl;
                        flag = 1;
                    }
                }
                if (flag == 1)
                    coordinator = subcoordinator;
                else
                    coordinator = activate;
                display();
                break;
            case 3:
                display();
                break;
            case 4:
                break;
        }
    } while (schoice != 4);
}

void ring() {
    int tchoice, crash, activate, gid, subcoordinator, i;
    do {
        cout << "\n1.CRASH\n2.ACTIVATE\n3.DISPLAY\n4.EXIT\n";
        cout << "Enter your choice: ";
        cin >> tchoice;
        switch (tchoice) {
            case 1:
                cout << "\nEnter Process ID to crash : ";
                cin >> crash;
                if (pStatus[crash]) {
                    pStatus[crash] = 0;
                } else {
                    cout << "Process " << crash << " is already dead!" << endl;
                }
                do {
                    cout << "Enter election generator id: ";
                    cin >> gid;
                    if (gid == coordinator) {
                        cout << "Please, enter a valid generator id.." << endl;
                    }
                } while (gid == coordinator);
                if (crash == coordinator) {
                    subcoordinator = 1;
                    for (i = 0; i < (n + 1); i++) {
                        int pid = (i + gid) % (n + 1);
                        if (pid != 0) {
                            if (pStatus[pid] && subcoordinator < pid) {
                                subcoordinator = pid;
                            }
                            cout << "Election message sent from " << pid << ": #Msg" << subcoordinator << endl;
                        }
                    }
                    coordinator = subcoordinator;
                }
                display();
                break;
            case 2:
                cout << "Enter Process ID to be activated: ";
                cin >> activate;
                if (!pStatus[activate]) {
                    pStatus[activate] = 1;
                } else {
                    cout << "Process " << activate << " is already alive!" << endl;
                    break;
                }
                subcoordinator = activate;
                for (i = 0; i < n + 1; i++) {
                    int pid = (i + activate) % (n + 1);
                    if (pid != 0) {
                        if (pStatus[pid] && subcoordinator < pid) {
                            subcoordinator = pid;
                        }
                        cout << "Election message passed from " << pid << ": #Msg" << subcoordinator << endl;
                    }
                }
                coordinator = subcoordinator;
                display();
                break;
            case 3:
                display();
                break;
            default:
                break;
        }
    } while (tchoice != 4);
}
