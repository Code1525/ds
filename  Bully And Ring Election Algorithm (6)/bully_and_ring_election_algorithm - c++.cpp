// first we include the necessary header files
#include <iostream>
#include <cstdlib>

// we define MAX as the maximum number of processes our program can simulate
// we declare array pStatus[MAX] to store the process status; 0 for dead and 1 for alive
// we declare n as the number of processes
// we declare coordinator to store the winner of election

#define MAX 20

int pStatus[MAX], n, coordinator;
using namespace std;

// we declare the necessary functions
void bully();
void ring();
// void ring_();   // this is also another approach ring implementation, and works well.
void display();

int main()
{
    int i, j, fchoice;
    cout << "Enter number of processes: ";
    cin >> n;
    for (i = 1; i <= n; i++)
    {
        cout << "Enter Process " << i << " is alive or not(0/1): ";
        cin >> pStatus[i];
        if (pStatus[i])
            coordinator = i;
    }
    display();
    do
    {
        cout << "-------------------------------------------------";
        cout << "\n1.BULLY ALGORITHM\n2.RING\n3.DISPLAY\n4.EXIT\n";
        cout << "-------------------------------------------------\n\n";
        cout << "Enter your choice: ";
        cin >> fchoice;
        switch (fchoice)
        {
        case 1:
            bully();
            break;
        case 2:
            ring();
            // ring_()
            break;
        case 3:
            display();
            break;
        case 4:
            exit(1);
            break;
        }
    } while (fchoice != 3);
    return 0;
}

void display()
{
    int i;
    // we display the processes, their status and the coordinator
    cout << "-------------------------------------------------\n";
    cout << "Processes:  ";
    for (i = 1; i <= n; i++) // PID from 1 to n
        cout << i << "\t";
    cout << endl
         << "Alive:      ";
    for (i = 1; i <= n; i++)
        cout << pStatus[i] << "\t";
    cout << "\n-------------------------------------------------\n";
    cout << "COORDINATOR IS " << coordinator << endl;
}

// bully algorithm implementation
void bully()
{
    int schoice, crash, activate, i, gid, flag, subcoordinator;
    do
    {
        cout << "-------------------------------------------------";
        cout << "\n1.CRASH\n2.ACTIVATE\n3.DISPLAY\n4.EXIT\n";
        cout << "-------------------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> schoice;
        switch (schoice)
        {
        case 1:
            // we manually crash the process to see if our implementation
            // can elect another coordinator
            cout << "Enter process to crash: ";
            cin >> crash;
            // if the process is alive then set its status to dead
            if (pStatus[crash])
                pStatus[crash] = 0;
            else
                cout << "Process " << crash << " is already dead!" << endl;
            do
            {
                // enter another process to initiate the election
                cout << "Enter election generator id: ";
                cin >> gid;
                if (gid == coordinator || pStatus[gid] == 0)
                    cout << "Please, enter a valid generator id.." << endl;
            } while (gid == coordinator || pStatus[gid] == 0);
            flag = 0;
            // if the coordinator has crashed then we need to find another coordinator
            if (crash == coordinator)
            {
                // the election generator process will send the message to all higher process
                for (i = gid + 1; i <= n; i++)
                {
                    cout << "Message  is sent from " << gid << " to " << i << endl;
                    // if the higher process is alive then it will respond
                    if (pStatus[i])
                    {
                        subcoordinator = i;
                        cout << "Response is sent from " << i << " to " << gid << endl;
                        flag = 1;
                    }
                }
                // the highest responding process is selected as the coordinator
                if (flag == 1)
                    coordinator = subcoordinator;
                // else if no higher process are alive then the election generator process
                // is selected as coordinator
                else
                    coordinator = gid;
            }
            display();
            break;
        case 2:
            // enter process to revive
            cout << "Enter Process ID to be activated: ";
            cin >> activate;
            // if the entered process was dead then it is revived
            if (!pStatus[activate])
            {
                pStatus[activate] = 1;
            }
            else
            {
                cout << "Process " << activate << " is already alive!" << endl;
                break;
            }
            // if the highest process is activated then it is the coordinator
            if (activate == n)
            {
                coordinator = n;
                break;
            }
            flag = 0;
            // else, the activated process sends message to all higher process
            for (i = activate + 1; i <= n; i++)
            {
                cout << "Message is sent from " << activate << " to " << i << endl;
                // if higher process is active then it responds
                if (pStatus[i])
                {
                    subcoordinator = i;
                    cout << "Response is sent from " << i << " to " << activate << endl;
                    flag = 1;
                }
            }
            // the highest responding process is made the coordinator
            if (flag == 1)
                coordinator = subcoordinator;
            // if no higher process respond then the activated process is coordinator
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

// ring algorithm implementation
void ring()
{
    int tchoice, crash, activate, gid, subcoordinator, i;
    do
    {
        cout << "-------------------------------------------------";
        cout << "\n1.CRASH\n2.ACTIVATE\n3.DISPLAY\n4.EXIT\n";
        cout << "-------------------------------------------------\n\n";
        cout << "Enter your choice: ";
        cin >> tchoice;
        switch (tchoice)
        {
        case 1:
            cout << "\nEnter Process ID to crash : ";
            cin >> crash;

            if (pStatus[crash])
                pStatus[crash] = 0;
            else
                cout << "Process " << crash << " is already dead!" << endl;
            do
            {
                cout << "Enter election generator id: ";
                cin >> gid;
                if (gid == coordinator)
                    cout << "Please, enter a valid generator id.." << endl;
            } while (gid == coordinator);

            if (crash == coordinator)
            {
                subcoordinator = 1;
                for (i = 0; i < (n + 1); i++)
                {
                    int pid = (i + gid) % (n + 1);
                    if (pid != 0) // since process id starts from 1 (to n)
                    {
                        if (pStatus[pid] && subcoordinator < pid)
                        {
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
            if (!pStatus[activate])
                pStatus[activate] = 1;
            else
            {
                cout << "Process " << activate << " is already alive!" << endl;
                break;
            }

            subcoordinator = activate;
            for (i = 0; i < n + 1; i++)
            {
                int pid = (i + activate) % (n + 1);

                if (pid != 0)
                {
                    if (pStatus[pid] && subcoordinator < pid)
                    {
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

// ring algorithm implementation (another approach)
// void ring_()
// {
//     int activePID[MAX], PID, k, temp;
//     int tchoice, crash, activate, i, generatorid, flag, subordinator;
//     do
//     {
//         cout << "-------------------------------------------------";
//         cout << "\n1.CRASH\n2.ACTIVATE\n3.DISPLAY\n4.EXIT\n";
//         cout << "-------------------------------------------------\n";
//         cout << "Enter your choice: ";
//         cin >> tchoice;
//         switch (tchoice)
//         {
//         case 1:
//             cout << "\nEnter Process to Crash: ";
//             cin >> crash;       // process to be crashed id is given
//             if (pStatus[crash]) // check whether process is alive or dead
//                 pStatus[crash] = 0;
//             else
//             {
//                 cout << "\nProcess is alreaady dead!!";
//                 break;
//             }
//             // election generation
//             do
//             {
//                 cout << "\nEnter election generator id:";

//                 cin >> generatorid;

//                 if (generatorid == coordinator) // check whether generator is cordinator
//                 {
//                     cout << "\nPlease,enter a valid generator id..." << endl;
//                 }
//             } while (generatorid == coordinator);
//             flag = 0;
//             k = 1;
//             if (crash == coordinator) // election is meaningful if crashed process is coordinator
//             {
//                 activePID[k] = generatorid;
//                 PID = generatorid;
//                 for (i = (generatorid + 1) % n; i != generatorid; i = (i + 1) % n)
//                 {
//                     if (i != 0)
//                     {
//                         if (pStatus[i])
//                         {
//                             cout << "\nMessage is sent from " << PID << " to " << i;
//                             activePID[k++] = i;
//                             PID++;
//                         }
//                     }
//                 }
//                 subordinator = 1;
//                 for (i = 1; i < k; i++)
//                 {
//                     if (subordinator < activePID[i])
//                     {
//                         subordinator = activePID[i];
//                     }
//                 }

//                 coordinator = subordinator;
//                 for (i = 1; i < k; i++)
//                 {
//                     cout << "\n"
//                          << coordinator << " is coordinator message to " << activePID[i] << endl;
//                 }
//             }
//             display();
//             break;

//         case 2:
//             // activate
//             cout << "\nEnter Process to Activate: ";
//             cin >> activate;
//             if (!pStatus[activate]) // activate the process
//                 pStatus[activate] = 1;
//             else
//             {
//                 cout << "\nProcess is already alive!" << endl;
//                 break;
//             }

//             if (activate == n) // if highest id process is activated
//             {
//                 coordinator = n; // then automatically it is coordinator
//                 break;
//             }
//             temp = activate;
//             for (i = (activate + 1) % (n + 1); i != activate; i = (i + 1) % (n + 1))
//             {
//                 if (i != 0)
//                 {
//                     cout << "Message is sent from " << temp << " to " << i << endl;
//                     if (pStatus[i] && i > activate)

//                     {
//                         subordinator = i;
//                         cout << "\nResponse is sent from " << i << " to " << temp << endl;
//                         flag = 1;
//                     }
//                     temp = (temp + 1) % (n + 1);
//                     if (temp == 0)
//                     {
//                         temp++;
//                     }
//                 }
//             }
//             if (flag == 1)
//             {
//                 coordinator = subordinator;
//             }
//             else
//             {
//                 coordinator = activate;
//             }
//             display();
//             break;
//         case 3:
//             display();
//             break;
//         case 4:
//             break;
//         }
//     } while (tchoice != 4);
// }
