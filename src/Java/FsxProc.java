package simApp;
import java.awt.Color;
import java.awt.Graphics;
import java.nio.ByteBuffer;
import javax.swing.JPanel;
/**
 * My Panel
 * @see http://docs.oracle.com/javase/tutorial/uiswing/painting/step3.html
 */
public class FsxProc extends JPanel {
	/**
	 * MyPanel
	 */
	private static final long serialVersionUID = 1L;
	private static final int    	serverPort = 5000;
	private static String	    	serverIP = "localhost";		//"192.168.2.101"
	private static FsxConnect   	fsxConnect;
	private static int  	    	nRequest = 0;
	//	parameters
	static final int
    	    			REQ_OBJECT_ID0 =0, REQ_PERIOD =1, REQ_FLAGS =0, REQ_ORIGIN =0,
    	    			REQ_INTERVAL =0, REQ_LIMIT =0;
	//	EVENT_ID
	static final int	EVENT_SIM_START =1, EVENT_PAUSE =3, EVENT_BRAKES=5;
	//	DATA_DEFINE_ID
	static final int	DEFINITION_1 =11, DEFINITION_2 =12, DEFINITION_3 =13, DEFINITION_X =13;
	//	DATA_REQUEST_ID
	static final int	REQUEST_1 =1, REQUEST_2 =2, REQUEST_3 =3;
	//	GROUP_ID
	static final int	GROUP_1 =1,  GROUP_2 =2,  GROUP_3 =3;


	FsxProc() {
		super();
		fsxConnect	= new FsxConnect();
	}

	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		g.drawString("This is my custom Panel!", 30, 20);
		g.setColor(Color.RED);
		g.fillRect( 20, 30, 250, 80);
	}

	public static void Connect() {
		fsxConnect.Connect("SimAppTest", serverIP, serverPort);
		fsxConnect.addToDataDefinition(DEFINITION_1, "Title", null , Sim.DataType.STRING256);
		fsxConnect.addToDataDefinition(DEFINITION_1, "Plane Latitude", "degrees");
		fsxConnect.addToDataDefinition(DEFINITION_1, "Plane Longitude", "degrees");
		fsxConnect.subscribeToSystemEvent( EVENT_SIM_START, "SimStart");
		fsxConnect.subscribeToSystemEvent( EVENT_PAUSE, "Pause" );
		fsxConnect.mapClientEventToSimEvent(EVENT_BRAKES, "brakes");
		fsxConnect.addClientEventToNotificationGroup(GROUP_1, EVENT_BRAKES);
		fsxConnect.setNotificationGroupPriority(GROUP_1, Sim.GroupPriority.HIGHEST);
	}

	/******************************************************
	 * submit an event
	 */
	public static void doEvent1() {
		fsxConnect.transmitClientEvent(Sim.OBJECT_ID_USER, EVENT_BRAKES, 0 , Sim.GroupPriority.HIGHEST, Sim.EventFlag.GROUPID_IS_PRIORITY);
	}

	/******************************************************
	 * request and receive location and direction information of plane.
	 */
	public static void requestData() {
		// e.g.  REQ,DEF=1,1  OBJ0=0=own plane, PERIOD/FLG/ORG/INT/LIM=1,0,0,0,0
		fsxConnect.requestDataOnSimObject( (++nRequest), DEFINITION_1, REQ_OBJECT_ID0,
							REQ_PERIOD, REQ_FLAGS, REQ_ORIGIN, REQ_INTERVAL, REQ_LIMIT);
	}

	public static void Disconnect() {
		fsxConnect.Close();
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	/**
	 * This is where all the data received from fsx SimConnect is delivered.
	 * FsxDispatchProc dispatches message records according to the RECV_ID
	 * code to the intended statements or method for processing
	 * @param bf message record content
	 */
	public static int FsxDispatchProc(ByteBuffer bf) {
		int iRecvID 	= ( bf.getInt() ) & 0x0fffffff;		// RECV_ID
		SimApp.displayText("RecvID="+iRecvID);
		return iRecvID;
	}

}
