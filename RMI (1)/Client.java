import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;
public class Client implements Runnable {
    private Service service;

    public Client(Service service) {
        this.service = service;
    }
    public void run() {
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.print("Enter message: ");
            String message = scanner.nextLine();
            try {
                service.receiveMessage(message);
            } catch (RemoteException e) {
                System.out.println("Client exception: " + e.toString());
                e.printStackTrace();
            }
        }
    }
    public static void main(String[] args) {
        try {
            Registry registry = LocateRegistry.getRegistry(1099);
            Service service = (Service) Naming.lookup("rmi://localhost/Service");
            Client client = new Client(service);
            Thread thread = new Thread(client);
            thread.start();
        } catch (Exception e) {
            System.out.println("Client exception: " + e.toString());
            e.printStackTrace();
        }
    }
}
