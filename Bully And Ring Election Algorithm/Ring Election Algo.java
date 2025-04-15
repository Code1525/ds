import java.util.*;

public class RingToken {
    
    public static class Process {
        int elected;
        int id;
        Process(int id, int elected) {
            this.id = id;
            this.elected = elected;
        }
    }

    public static void main(String args[]) {
        List<Process> processes = new ArrayList<>();

        Random rand = new Random();

        // initial case with 6th process as coordinator
        for(int i=0;i<5;i++) {
            processes.add(new Process(rand.nextInt(100), 200));
        }
        processes.add(new Process(200, 200));
        Collections.shuffle(processes);
        
        System.out.println("Initial Process Ring");
        print(processes);

        // P=5 Fails
        // New Leader to be Elected

        // Process Starting Election is just before P=5         
        int initial_process_index = 5;
        for(int i=0;i<5;i++) {
            if (processes.get(i+1).id == 200) {
                initial_process_index = i;
                break;
            }
        }

        // Current Process Same as Election Starting Process
        // Starting process sets message as its own ID
        int current_process_index = initial_process_index;
        int message = processes.get(current_process_index).id;
        
        System.out.println("\nStarting Process Index " + current_process_index + "\n");
        
        current_process_index++;
        current_process_index%=6;

        System.out.println("Process Ring While Finding Max ID");
        print(processes, message);

        // Finding Max ID 
        do {
            
            int cur = current_process_index;
            
            // Exclude Failed Process
            if (processes.get(cur).id != 200) {
                
                // If own ID is bigger than previous ID then set it as own message
                if (processes.get(cur).id > message) {
                    message = processes.get(cur).id;
                }

                print(processes, message);

            }

            current_process_index++;
            current_process_index%=6;

        } while (current_process_index != initial_process_index);

        int maxid = message;
        
        // Finding Coordinator with max ID
        while(processes.get(current_process_index).id != maxid) {
            current_process_index++;
            current_process_index%=6;
        }

        int coordinator_process = current_process_index;
        initial_process_index = coordinator_process;

        System.out.println("\nProcess Ring While Sending Coordinator Message");
        // Send Coordinator Message & Set elected
        do {
            processes.get(current_process_index).elected = processes.get(coordinator_process).id; 
            print(processes);
            current_process_index++;
            current_process_index%=6;
        } while (current_process_index != initial_process_index);

        System.out.println("\nFinal Process Ring");
        print(processes);

        System.out.println("\nCoordinator Process Index " + coordinator_process + "\n");

    }

    public static void print(List<Process> processes, int message) {
        System.out.println("---------------------");
        System.out.print("P.I ");
        for(int i=0;i<6;i++) {
            System.out.print(processes.get(i).id + " ");
        }
        System.out.print("\nP.M " + message);
        System.out.print("\nP.E ");
        for(int i=0;i<6;i++) {
            System.out.print(processes.get(i).elected + " ");
        }
        System.out.println("\n---------------------");
    }

    public static void print(List<Process> processes) {
        System.out.println("---------------------");
        System.out.print("P.I ");
        for(int i=0;i<6;i++) {
            System.out.print(processes.get(i).id + " ");
        }
        System.out.print("\nP.E ");
        for(int i=0;i<6;i++) {
            System.out.print(processes.get(i).elected + " ");
        }
        System.out.println("\n---------------------");
    }
}