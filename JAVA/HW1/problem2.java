
public class problem2 {
	public static void main(String[] args) {
		for(int i = 1; i<9; i++) {
			for(int j = 0; j<i-1; j++) {
				System.out.print(" ");
			}
			System.out.print("*");
			for(int j = 0; j<2 * (9-i)-1; j++) {
				System.out.print(" ");
			}
			System.out.println("*");
		}
		System.out.println("        *");
		for(int i = 1; i<9; i++) {
			for(int j = 0; j<8-i; j++) {
				System.out.print(" ");
			}
			System.out.print("*");
			for(int j = 0; j<2 * i-1; j++) {
				System.out.print(" ");
			}
			System.out.println("*");
		}
	}
}
