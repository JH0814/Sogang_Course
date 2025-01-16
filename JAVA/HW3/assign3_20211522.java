package assign3;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
class WindowDestroyer extends WindowAdapter
{
    public void windowClosing(WindowEvent e) 
    {
        System.exit(0);
    }
}
public class assign3_20211522 extends Frame implements ActionListener{ 
	public Canvas canvas;
	// 초기 속도 정하는 array
	private static int[] dx_arr = {1, 1, -1, -1, 0};
	private static int[] dy_arr = {-1, 0, 1, 0, -1};
	private static Vector<Ball> b_arr; // 공 저장 Vector
	public assign3_20211522(){ 
		canvas = new Canvas();
		add("Center", canvas);
		Panel p = new Panel();
		Button s = new Button("Start");
		Button c = new Button("Close");
		p.add(s); p.add(c);
		s.addActionListener(this);
		c.addActionListener(this);
		add("South", p);
		Ball ball;
		// 초기 공 생성
		b_arr = new Vector<>();
		for(int i = 0; i<5; i++) {
			ball = new Ball(canvas, dx_arr[i], dy_arr[i], 16, 16);
			b_arr.add(ball);
		}
	}
	public void actionPerformed(ActionEvent evt){ 
		if (evt.getActionCommand() == "Close")
			System.exit(0);
	}
	public static void main(String[] args){ 
		Frame f = new assign3_20211522();
		f.setSize(400, 300);
		WindowDestroyer listener = new WindowDestroyer();
		f.addWindowListener(listener);
		f.setVisible(true);
		Dimension d = f.getSize();
		for(int i = 0; i<b_arr.size(); i++) { // Thread 시작
			b_arr.get(i).x = d.width/2 + dx_arr[i] * 40 - 7;
			b_arr.get(i).y = d.height/2 + dy_arr[i] * 40 - 30;
			b_arr.get(i).start();
		}
	}
	private Canvas box;
	
	class Ball extends Thread{
		public int x;
		public int y;
		private int dx;
		private int dy;
		private int XSIZE;
		private int YSIZE;
		public Ball(Canvas c, int d_x, int d_y, int size_x, int size_y) { 
			box = c;
			dx = d_x;
			dy = d_y;
			XSIZE = size_x;
			YSIZE = size_y;
		}
		public void draw(){ 
			Graphics g = box.getGraphics();
			g.fillOval(x, y, XSIZE, YSIZE);
			g.dispose(); 
		}
		public void move(){ 
			Graphics g = box.getGraphics();
			g.setColor(Color.WHITE);
			g.fillOval(x, y, XSIZE, YSIZE);
			x += dx; 
			y += dy;
			Dimension d = box.getSize();
			int arr_size = b_arr.size();
			if (x < 0) { x = 0; dx = -dx; }
			if (x + XSIZE >= d.width) { x = d.width - XSIZE; dx = -dx; }
			if (y < 0) { y = 0; dy = -dy; }
			if (y + YSIZE >= d.height) { y = d.height - YSIZE; dy = -dy; }
			if(XSIZE != 1)
				g.setColor(Color.BLACK);
			else
				g.setColor(Color.WHITE);
			g.fillOval(x, y, XSIZE, YSIZE);
			if(XSIZE > 1) { // 공 크기가 1보다 클 때만 충돌 고려
				for(int i = 0; i<arr_size; i++) {
					if(collide(i)) {
						// 속도를 적절히 반대로 바뀌도록 조정
						if(dx != 0)
							dx *= -1;
						else
							dx = -1;
						dy = 1;
						XSIZE /= 2;
						YSIZE /= 2;
						Ball b = b_arr.get(i);
						if(b.dx != 0)
							b.dx *= -1;
						else
							b.dx = 1;
						b.dy = 1;
						b.XSIZE /= 2;
						b.YSIZE /= 2;
						if(XSIZE != 1){
							int ran = (int) (Math.random()*4);
							int tmp_dx;
							if(ran < 2) {
								tmp_dx = dx * 2;
							}
							else {
								tmp_dx = dx;
							}
							// 해당 Thread의 공의 1/2 크기 공 복제
							Ball ball1 = new Ball(box, tmp_dx, -1, XSIZE, YSIZE);
							ball1.x = x + dx * (2 * XSIZE + 1);
							ball1.y = y;	
							b_arr.add(ball1);
							ball1.start();
						}
						if(b.XSIZE != 1){
							int ran = (int) (Math.random()*3);
							int tmp_dy;
							if(ran < 1) {
								tmp_dy = -1;
							}
							else {
								tmp_dy = -2;
							}
							// 해당 Thread의 공과 부딫힌 공의 1/2 크기 복제
							Ball ball2 = new Ball(box, b.dx, tmp_dy, b.XSIZE, b.YSIZE);
							ball2.x = b.x;
							ball2.y = b.y + b.dy*(2 * b.YSIZE + 1);	
							b_arr.add(ball2);
							ball2.start();
						}
						g.setColor(Color.WHITE);
						g.fillRect(0, 0, box.getWidth(), box.getHeight());
					}
				}
			}
			g.dispose();
		}
		public void run(){ 
			draw();
			for (;XSIZE > 1;){ 
				move();
				try { Thread.sleep(5); } 
				catch(InterruptedException e) {}
				// 종료 조건 판정
				int flag = 0;
				int cnt = 0;
				int sec_cnt = 0;
				for(int i = 0; i<b_arr.size(); i++) {
					if(b_arr.get(i).XSIZE > 1) { // 1보다 큰 공 있을 때 flag 1로 바꿈
						flag = 1;
						cnt++;
						if(b_arr.get(i).XSIZE == 2) {
							sec_cnt++;
						}
					}
				}
				if(flag == 0 || (cnt == 1 && sec_cnt == 1)) { // 공이 모두 크기가 1이면 종료
					System.exit(0);
				}
			}
			// 모두 크기가 1이 되면 종료 한 번 더 확인
			int flag = 0;
			for(int i = 0; i<b_arr.size(); i++) {
				if(b_arr.get(i).XSIZE > 1) {
					flag = 1;
					break;
				}
			}
			if(flag == 0) {
				System.exit(0);
			}
		}
		public boolean collide(int i) { // 충돌 판정하는 function
			Ball b = b_arr.get(i);
			int nx = b.x;
			int ny = b.y;
			int dis = (x - nx) * (x - nx) + (y - ny) * (y - ny);
			if(b.XSIZE > 1 && dis > 0 && dis <= (XSIZE + b.XSIZE) * (XSIZE + b.XSIZE)) {
				return true;
			}
			else {
				return false;
			}
		}
	}
}