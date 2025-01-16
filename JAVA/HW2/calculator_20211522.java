package HW2;

// x! : (숫자) 입력 후 버튼 입력, = 입력 후 결과 출력으로 구현

// % : (숫자) 입력 후 버튼 입력, = 입력 후 결과 출력으로 구현

// sin, cos, tan : 버튼 입력 후 (숫자) 입력, = 입력 후 결과 출력으로 구현

// ln, log : 버튼 입력 후 (숫자) 입력, = 입력 후 결과 출력으로 구현

// √ : "√" 버튼 입력 후 (숫자) 입력, = 입력 후 결과 출력으로 구현

// x ^ y : (숫자) 입력, 버튼 입력, (숫자) 입력 후 = 입력으로 결과 출력으로 구현

import java.awt.*; 
import java.awt.event.*;
import java.util.Stack;

class WindowDestroyer extends WindowAdapter
{
    public void windowClosing(WindowEvent e) 
    {
        System.exit(0);
    }
}

public class calculator_20211522 implements ActionListener{
	private Frame f;
	private String formula = "";
	private Label pre_form = new Label("");
	private Label res_form = new Label("0");
	private int fir_flag = 0;
	private static String[] btn_list = {"", "x!", "(", ")", "%", "AC", 
										"sin", "ln", "7", "8", "9", "/",
										"cos", "log", "4", "5", "6", "*",
										"tan", "√", "1", "2", "3", "-",
										"", "x^y", "0", ".", "=", "+"};
	double result = 0.0;
	int flag = 0;
	public static void main(String[] args) {
		calculator_20211522 calc = new calculator_20211522();
		calc.set_display();
	}
	public void set_display() {
		f = new Frame("Calculator");
		Panel res_panel = new Panel();
		res_panel.setLayout(new GridLayout(2, 1));
		pre_form.setAlignment(Label.RIGHT);
		res_form.setAlignment(Label.RIGHT);
		res_panel.add(pre_form);
		res_panel.add(res_form);
		f.add(res_panel, BorderLayout.NORTH);
		Panel btn_panel = new Panel();
		btn_panel.setLayout(new GridLayout(5, 6));
		for(int i = 0; i<btn_list.length; i++) {
			Button tmp = new Button(btn_list[i]);
			tmp.addActionListener(this);
			btn_panel.add(tmp);
		}
		f.add(btn_panel);
		WindowDestroyer listener = new WindowDestroyer();
		f.addWindowListener(listener);
		f.pack();
		f.setVisible(true);
	}
	@Override
	public void actionPerformed(ActionEvent e) {
		switch(e.getActionCommand()) {
		case "AC":
			pre_form.setText("");
			res_form.setText("0");
			formula = "";
			flag = 0;
			fir_flag = 0;
			break;
		case "=":
			if(Character.isDigit(formula.charAt(formula.length()-1)) || formula.charAt(formula.length()-1) == '%' || formula.charAt(formula.length()-1) == '!'
			|| formula.charAt(formula.length()-1) == ')') {
				pre_form.setText(formula);
				if(calc(formula, flag)) {
					String r = String.format("%.8f", result);
					int i;
					for(i = r.length() - 1; i>=0; i--) {
						if(r.charAt(i) != '0' || r.charAt(i-1) == '.') {
							break;
						}
					}
					r = r.substring(0, i + 1);
					if(r.charAt(r.length()-1) == '0' && r.charAt(r.length()-2) == '.'){
						int x = r.length();
						r = r.substring(0, x - 2);
					}
					res_form.setText(r);
				}
				else {
					res_form.setText("Calculation has error");
				}
				formula = "";
				flag = 0;
				fir_flag = 0;
			}
			break;
		case "sin":
		case "cos":
		case "tan":
			flag = 1;
			formula += e.getActionCommand();
			res_form.setText(formula);
			fir_flag = 1;
			break;
		case "%":
			flag = 2;
			formula += e.getActionCommand();
			res_form.setText(formula);
			fir_flag = 1;
			break;
		case "log":
		case "ln":
			flag = 3;
			formula += e.getActionCommand();
			res_form.setText(formula);
			fir_flag = 1;
			break;
		case "√":
			flag = 4;
			formula += "√";
			res_form.setText(formula);
			fir_flag = 1;
			break;
		case "x^y":
			flag = 5;
			formula += "^";
			res_form.setText(formula);
			fir_flag = 1;
			break;
		case "x!":
			flag = 6;
			if(formula == "") {
				formula += "0";
			}
			formula += "!";
			res_form.setText(formula);
			fir_flag = 1;
			break;
		case "+":
		case "-":
		case "*":
		case "/":
		case "(":
		case ")":
			formula += e.getActionCommand();
			res_form.setText(formula);
			fir_flag = 1;
			break;
		case "":
			break;
		case "0":
			if(!(fir_flag == 0 || (formula.charAt(formula.length() - 1) == '0' && e.getActionCommand().equals("0")))) {
				formula += e.getActionCommand();
				res_form.setText(formula);
				fir_flag = 1;
			}
			break;
		default:
			if((formula.length() == 0 || !Character.isDigit(formula.charAt(formula.length() - 1))) && e.getActionCommand()==".") {
				formula += "0";
			}
			formula += e.getActionCommand();
			res_form.setText(formula);
			fir_flag = 1;
			break;
		}
	}
	public boolean calc(String p, int flag) {
		if(flag == 1) {
			if(p.substring(0, 3).equals("sin")) {
				result = Math.sin(Math.toRadians(Double.valueOf(p.substring(3))));
			}
			else if(p.substring(0, 3).equals("cos")) {
				result = Math.cos(Math.toRadians(Double.valueOf(p.substring(3))));
			}
			else if(p.substring(0, 3).equals("tan")) {
				result = Math.tan(Math.toRadians(Double.valueOf(p.substring(3))));
			}
			else {
				return false;
			}
			return true;
		}
		if(flag == 2) {
			result = Double.valueOf(p.substring(0, p.length() - 1))/100.0;
			return true;
		}
		if(flag == 3) {
			if(p.substring(0, 2).equals("ln")) {
				result = Math.log(Double.valueOf(p.substring(2)));
			}
			else if(p.substring(0, 3).equals("log")) {
				result = Math.log(Double.valueOf(p.substring(3)))/Math.log(10.0);
			}
			else {
				return false;
			}
			return true;
		}
		if(flag == 4) {
			if(p.charAt(0) == '√') {
				result = Math.sqrt(Double.valueOf(p.substring(1)));
				return true;
			}
			else {
				return false;
			}
		}
		if(flag == 5) {
			int idx = p.indexOf("^");
			if(idx != -1) {
				result = Math.pow(Double.valueOf(p.substring(0, idx)), Double.valueOf(p.substring(idx+1)));
				return true;
			}
			else {
				return false;
			}
		}
		if(flag == 6) {
			if(p.charAt(p.length()-1) == '!') {
				result = 1.0;
				for(int i = Integer.valueOf(p.substring(0, p.length() - 1)); i>=1; i--) {
					result *= i;
				}
				return true;
			}
			else {
				return false;
			}
		}
		int cnt = 0;
		for(int i = 0; i<p.length(); i++) {
			char c = p.charAt(i);
			if(c == '+' || c == '-' || c == '*' || c == '/') {
				cnt++;
				if(Character.isDigit(p.charAt(i+1)) || p.charAt(i+1) == '(') {
					if(cnt == 1) {
						cnt = 0;
						continue;
					}
					else {
						if(c == '-') {
							p = p.substring(0, i+1-cnt) + p.substring(i-1);
							i -= cnt-2;
						}
						else {
							p = p.substring(0, i+1-cnt) + p.substring(i);
							i -= cnt-1;
						}
						cnt=0;
					}
				}
			}
		}
		Stack<Character> check = new Stack<>();
		for(int i = 0; i<p.length(); i++) {
			if(p.charAt(i) == '(') {
				check.add(p.charAt(i));
			}
			if(p.charAt(i) == ')') {
				if(check.isEmpty()) {
					return false;
				}
				else {
					check.pop();
				}
			}
		}
		if(!check.isEmpty()) {
			return false;
		}
		Stack<Double> num_stack = new Stack<>();
		Stack<Character> op_stack = new Stack<>();
		String num = "";
		for(int i = 0; i<p.length(); i++) {
			char c = p.charAt(i);
			if(c == '(') {
				op_stack.add(c);
				num = "";
				continue;
			}
			if(c == ')') {
				if(num != "") {
					num_stack.add(Double.valueOf(num));
				}
				if(!op_stack.isEmpty() && (op_stack.peek().equals('*')|| op_stack.peek().equals('/'))) {
					double n1 = num_stack.pop();
					double n2 = num_stack.pop();
					Character op = op_stack.pop();
					if(op.equals('*')) {
						num_stack.add(n1 * n2);
					}
					else if(op.equals('/')) {
						if(n1 == 0) {
							return false;
						}
						else {
							num_stack.add(n2 / n1);
						}
					}
				}
				Stack<Double> num_stack_rep = new Stack<>();
				Stack<Character> op_stack_rep = new Stack<>();
				num_stack_rep.add(num_stack.pop());
				while(op_stack.peek() != '(') {
					num_stack_rep.add(num_stack.pop());
					op_stack_rep.add(op_stack.pop());
				}
				op_stack.pop();
				while(!op_stack_rep.isEmpty()) {
					double n1 = num_stack_rep.pop();
					double n2 = num_stack_rep.pop();
					Character op = op_stack_rep.pop();
					if(op.equals('+')) {
						num_stack_rep.add(n1 + n2);
					}
					else if(op.equals('-')) {
						num_stack_rep.add(n1 - n2);
					}
				}
				num_stack.add(num_stack_rep.pop());
				num = "";
				while(!op_stack.isEmpty() && (op_stack.peek().equals('*')|| op_stack.peek().equals('/'))) {
					double n1 = num_stack.pop();
					double n2 = num_stack.pop();
					Character op = op_stack.pop();
					if(op.equals('*')) {
						num_stack.add(n1 * n2);
					}
					else if(op.equals('/')) {
						if(n1 == 0) {
							return false;
						}
						else {
							num_stack.add(n2 / n1);
						}
					}
				}
				continue;
			}
			if(c == '-'|| c == '+' || c == '*' || c == '/') {
				if(i == 0 && c == '-') {
					num += c;
					continue;
				}
				if(c == '-' && !Character.isDigit(p.charAt(i-1))) {
					num += c;
					continue;
				}
				if(num != "") {
					num_stack.add(Double.valueOf(num));
				}
				if(!op_stack.isEmpty() && (op_stack.peek().equals('*')|| op_stack.peek().equals('/'))) {
					double n1 = num_stack.pop();
					double n2 = num_stack.pop();
					Character op = op_stack.pop();
					if(op.equals('*')) {
						num_stack.add(n1 * n2);
					}
					else if(op.equals('/')) {
						if(n1 == 0) {
							return false;
						}
						else {
							num_stack.add(n2 / n1);
						}
					}
				}
				op_stack.add(c);
				num = "";
			}
			else {
				num += c;
			}
			if(i == p.length() - 1) {
				if(c == '-'|| c == '+' || c == '*' || c == '/') {
					op_stack.pop();
				}
				if(!op_stack.isEmpty() && (op_stack.peek().equals('*')|| op_stack.peek().equals('/'))) {
					double n1 = Double.valueOf(num);
					double n2 = num_stack.pop();
					Character op = op_stack.pop();
					if(op.equals('*')) {
						num_stack.add(n1 * n2);
					}
					else if(op.equals('/')) {
						if(n1 == 0) {
							return false;
						}
						else {
							num_stack.add(n2 / n1);
						}
					}
				}
				else {
					if(num != "") {
						num_stack.add(Double.valueOf(num));
					}
				}
			}
		}
		while(!op_stack.isEmpty() && (op_stack.peek().equals('*')|| op_stack.peek().equals('/'))) {
			double n1 = num_stack.pop();
			double n2 = num_stack.pop();
			Character op = op_stack.pop();
			if(op.equals('*')) {
				num_stack.add(n1 * n2);
			}
			else if(op.equals('/')) {
				if(n1 == 0) {
					return false;
				}
				else {
					num_stack.add(n2 / n1);
				}
			}
		}
		Stack<Double> num_stack_rep = new Stack<>();
		Stack<Character> op_stack_rep = new Stack<>();
		while(!num_stack.isEmpty()) {
			num_stack_rep.add(num_stack.pop());
		}
		while(!op_stack.isEmpty()) {
			op_stack_rep.add(op_stack.pop());
		}
		while(!op_stack_rep.isEmpty() && num_stack_rep.size() >= 2) {
			double n1 = num_stack_rep.pop();
			double n2 = num_stack_rep.pop();
			Character op = op_stack_rep.pop();
			if(op == '+') {
				num_stack_rep.add(n1 + n2);
			}
			else if(op == '-') {
				num_stack_rep.add(n1 - n2);
			}
		}
		if(num_stack_rep.size() == 1 && op_stack_rep.isEmpty()) {
			result = num_stack_rep.pop();
			return true;
		}
		else {
			return false;
		}
	}
}
