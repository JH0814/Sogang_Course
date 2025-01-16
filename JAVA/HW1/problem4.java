
import java.util.Scanner;

public class problem4 {
	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);
		String s = sc.nextLine();
		for(int i = s.length() - 1; i>=0; i--) {
			System.out.print(s.substring(i, i + 1));
		}
		System.out.println();
	}
}
