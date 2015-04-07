// Client for the Tennis Server
// See full notes at head of server listing.
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import java.io.*;
import javax.swing.*;
import java.util.Scanner;

// Client class to let a user play Tennis with another user across a network.
public class TennisClient extends JFrame implements Runnable {
   private JPanel court;
   private JButton stopBtn;
   private Socket connection;
   private DataInputStream input;
   private DataOutputStream output;
   private Thread outputThread;
   private boolean running;
   private int ballX, ballY, ballRad, ballDia, topRacX, btmRacX, topRacY, btmRacY;
   private final int ctWdth = 300, ctHgt = 500, racOffs = 10, racWidth = 20;
   public static final int 	DONE 	= 0x0D03,
							NEWBALL	= 0x0BA1,
							RAQX 	= 0x1000,
							RAQY 	= 0x2000;

   // Set up user-interface
   public TennisClient(String address, int port)   {
      super("Tennis Client");
      ballX = ballY = -10; ballRad = 5; 
      ballDia = 2*ballRad;
      topRacX = btmRacX = ctWdth/2;
	  topRacY = racOffs-1;
	  btmRacY = ctHgt-racOffs;
      
      court = new JPanel() {
        public void paintComponent(Graphics g) {
          super.paintComponent(g);
          g.drawRect(0, 0, ctWdth, ctHgt);
          g.setColor(Color.blue);
          g.fillRect(topRacX-racWidth, topRacY, 2*racWidth, 2);
          g.fillRect(btmRacX-racWidth, btmRacY, 2*racWidth, 2);
          g.setColor(Color.red);
          g.fillOval(ballX-ballRad, ballY-ballRad, ballDia, ballDia);
        }
      };
      court.setPreferredSize(new Dimension(ctWdth, ctHgt));
      court.addMouseMotionListener(new MouseMotionAdapter() {
        public void mouseMoved(MouseEvent ev) {
          try {
			int outX = (ev.getX() & 0xFFF) + 2000;
			int outY = (ev.getY() & 0xFFF) + 2000;

			outX |= RAQX;
			outY |= RAQY;

            output.writeChar(outX);
			output.writeChar(outY);

// 			System.out.println("Sending [" + outX + ":" + outY + "]");
          } catch(IOException ex) {
            ex.printStackTrace();
          }
        }
      });
      court.addMouseListener(new MouseAdapter() {
        public void mouseClicked(MouseEvent ev) {
          try {
            output.writeChar(NEWBALL);
          } catch(IOException ex) {
            ex.printStackTrace();
          }
        }
      });
      getContentPane().add(court, BorderLayout.CENTER);

      stopBtn = new JButton("Stop");
      stopBtn.addActionListener(new ActionListener() {
        public void actionPerformed(ActionEvent ev) {
          try {
            output.writeChar(DONE);
          } catch(IOException ex) {
            ex.printStackTrace();
          }
        }
      });
      getContentPane().add(stopBtn, BorderLayout.NORTH);
      addWindowListener(new WindowAdapter() {
        public void windowClosing(WindowEvent ev) {
          stopBtn.doClick();
        }
      });
      setSize(ctWdth+30, ctHgt+80);
      setVisible(true);
      setResizable(false);

      // Make connection to server and get associated streams. Start separate
      // thread to allow this applet to continually update its appearance.
      try {
         connection = new Socket(InetAddress.getByName(address), port);
         connection.setTcpNoDelay(true); //disable Nagle's Algorithm
         input = new DataInputStream(connection.getInputStream());
         output = new DataOutputStream(connection.getOutputStream());
      }
      catch (IOException e) {
         e.printStackTrace();         
      }

      outputThread = new Thread(this);
      outputThread.start();
   }

   public boolean isRunning() { return running; }

	// Control thread that updates display from received messages.
	public void run()
	{
		running = true;
		// Receive messages sent to client
		while (running) 
		{
			try
			{
				byte[] buff = new byte[255];
				int c, len;
			
				c = 0;
				len = input.read(buff);

				if (len > 0)
				{
					processMessage(new String(buff, 0, Math.min(250,len) ));
				}
			} catch (IOException e) { System.err.println(e); }
		}

		System.out.println("Goodbye from player");
		System.exit(0);
	} //end run()

	// Process messages sent to client: 
	// POS <ballX> <ballY>
	// RAC <top racquet X> <btm racquet X> <top racquet Y> <btm racquet Y>
   public void processMessage(String input)   {

      if (input.equals("DONE")) {
        running = false;
        return;
      }
      Scanner sc = new Scanner(input);
      sc.useDelimiter(" ");
      if (!sc.hasNext()) 
		return;

      String stg = sc.next();
		
	  //ball position msg 
      if (stg.equals("POS")) 
	  { 
		// System.out.println("---- " + input);

        if (sc.hasNextInt())          ballX = sc.nextInt();
        if (sc.hasNextInt())          ballY = sc.nextInt();

		// System.out.println("____ [" + ballX + ":" + ballY + "]");
      }
	  //racquet posns 
      else if (stg.equals("RAC")) 
	  { 
        if (sc.hasNextInt())          topRacX = sc.nextInt();
        if (sc.hasNextInt())          btmRacX = sc.nextInt();
        if (sc.hasNextInt())          topRacY = sc.nextInt();
        if (sc.hasNextInt())          btmRacY = sc.nextInt();
      }

      court.repaint();
   }

   public static void main(String[] args) {
     if (args.length == 0) {
        System.out.println("Usage: java TennisClient <address> <port>");
        System.out.println("<address> is rrequired; default <port> is 4000");
        return;
     }
     int port = 4000;
     if (args.length > 1)        	port = Integer.parseInt(args[1]);

     TennisClient client = new TennisClient(args[0], port);
   }
}
