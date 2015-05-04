package simApp;
import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.ComponentOrientation;
import java.awt.Dimension;
import java.awt.EventQueue;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.SwingConstants;
import javax.swing.border.EmptyBorder;
/**
 * A Swing GUI application inherits from top-level container javax.swing.JFrame
 * @see http://www.ntu.edu.sg/home/ehchua/programming/java/J4a_GUI.html
 */
class SimApp extends JFrame {
	/**
	 * Swing graphics test
	 */
	private static final long	serialVersionUID = 1L;
	private static final Dimension	BUTTON_SIZE = new Dimension(150, 25);
	private static final Dimension	SPACE_SIZE = new Dimension(0, 10);
	private static final Dimension	DRAW_SIZE = new Dimension(0, 150);
	private JPanel		    	contentPane, buttonPanel, drawPanel;
	private JLabel		    	lblTitle;
	private JButton		    	btnConnect, btnEvent1, btnReqData, btnDisconnect;
	private static JTextArea	textArea;
	private static String   	displayTextBuffer = "\n00\n01\n02\n03\n04\n05\n06\n07\n08";
	private static int	    	displayLineNumber = 0;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			@Override
			public void run() {
				//System.out.println("start event thread");
				try {
					SimApp frame = new SimApp();
					frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		//System.out.println("main is ready");
	}

	/**
	 * prepare a numbered multi-line text log for displaying
	 * @param txt text to add as new line to the end
	 * @return multi-line text
	 */
	public static void displayText(String txt) {
		displayTextBuffer = displayTextBuffer.substring(displayTextBuffer.indexOf('\n')+1)
				+"\n"+(++displayLineNumber)+": " +txt;
		textArea.setText(displayTextBuffer);
	}

	/**
	 * Constructor: Create the frame.
	 */
	public SimApp() {
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setTitle("SimConnect Demo");

		contentPane	= new JPanel();
		contentPane.setComponentOrientation(ComponentOrientation.LEFT_TO_RIGHT);
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setBounds(100, 100, 450, 400);
		setContentPane(contentPane);
		contentPane.setLayout(new BorderLayout(5, 5));

		// title at top
		lblTitle	= new JLabel("SimConnect demo");
		lblTitle.setHorizontalAlignment(SwingConstants.CENTER);
		contentPane.add(lblTitle, BorderLayout.PAGE_START);

		// the button panel
		buttonPanel	= new JPanel();
		buttonPanel.setOpaque(false);
		buttonPanel.setBorder(new EmptyBorder(0, 10, 0, 10));
		buttonPanel.setAlignmentY(Component.TOP_ALIGNMENT);
		buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.Y_AXIS));
		buttonPanel.add(Box.createRigidArea(SPACE_SIZE));
		contentPane.add(buttonPanel, BorderLayout.LINE_START);

		// the buttons
		btnConnect	= new JButton("Connect");
		btnConnect.setPreferredSize(BUTTON_SIZE);
		btnConnect.setMaximumSize(BUTTON_SIZE);
		buttonPanel.add(btnConnect);
		buttonPanel.add(Box.createRigidArea(SPACE_SIZE));

		btnEvent1	= new JButton("Event1");
		btnEvent1.setMaximumSize(BUTTON_SIZE);
		buttonPanel.add(btnEvent1);
		buttonPanel.add(Box.createRigidArea(SPACE_SIZE));

		btnReqData	= new JButton("RequestData");
		btnReqData.setMaximumSize(BUTTON_SIZE);
		buttonPanel.add(btnReqData);
		buttonPanel.add(Box.createRigidArea(SPACE_SIZE));

		btnDisconnect	= new JButton("Disconnect fsx");
		btnDisconnect.setMaximumSize(BUTTON_SIZE);
		buttonPanel.add(btnDisconnect);
		buttonPanel.add(Box.createRigidArea(SPACE_SIZE));

		// the textbox
		textArea	= new JTextArea();
		contentPane.add(textArea, BorderLayout.CENTER);

		// bottom
		drawPanel	= new FsxProc();
		drawPanel.setPreferredSize(DRAW_SIZE);
		drawPanel.setLayout(null);
		contentPane.add(drawPanel, BorderLayout.SOUTH);

		// button actions
		btnConnect.   addActionListener( e -> FsxProc.Connect()     );
		btnEvent1.    addActionListener( e -> FsxProc.doEvent1()    );
		btnReqData.   addActionListener( e -> FsxProc.requestData() );
		btnDisconnect.addActionListener( e -> FsxProc.Disconnect()  );
	}
}
