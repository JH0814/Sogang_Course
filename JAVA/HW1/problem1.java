
public class problem1 {

	public static void main(String[] args) {
		while(true) {
			int die1 = (int)(6*Math.random() + 1);
			int die2 = (int)(6*Math.random() + 1);
			System.out.print("("+die1+", "+die2+") ");
			if(die1 + die2 == 7) {
				break;
			}
		}

	}

}
