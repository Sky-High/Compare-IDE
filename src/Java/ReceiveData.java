package simApp;
import java.util.List;
import java.util.concurrent.ExecutionException;
import javax.swing.SwingWorker;
/**
 * read data in a worker thread
 */
public class ReceiveData extends SwingWorker<Integer, Integer> {

	/******************************************************
	 * This method runs in a separate worker thread in order
	 * not to disturb the UI thread or EDT Event Dispatcher Thread.
	 *<p>
	 * In a continuous loop wait for and read next messages from 
	 * the socket channel until the socket is closed or the
	 * worker thread is canceled.
	 *<p>
	 * The received message is parsed and a notification is sent
	 * by 'publish' to the EDT for further event and UI handling.
	 */
	@Override
	public Integer doInBackground() throws Exception {
		int nxt;
		do {
			nxt = FsxConnect.readRecord(FsxConnect.CM_READ_AGAIN);
			// publish the progress to the 'process' method below
			// to process the data in the EDT thread.
			publish(nxt);
		} while (nxt != 0 && !this.isCancelled() );
		return nxt;
	}

	/******************************************************
	 * This callback method runs in the Event Dispatch Thread.
	 * It asynchronously receives data chunks from the publish method
	 * in the above 'doInBackground' method, and will process the
	 * concerning data.
	 * @param chunks a list of one or more data items received
	 *               from 'publish()' in 'doInBackground'
	 */
	@Override
	public void process( List<Integer> chunks ) {
		SimApp.displayText("<rcvp="+chunks+ ">");
	}

	/******************************************************
	 * This callback method runs in the Event Dispatch Thread. 
	 * It is called when 'doInBackground' has completed its
	 * work, and exits. The worker thread will then stop.
	 *<p>
	 * This 'done' method calls 'get()' to read the final status
	 * from 'doInBackground' and will perform possible post
	 * processing.
	 */
	@Override
	public void done() {
		//in Event Dispatcher Thread. worker thread has ended
		String	msg = "exception";
		try {
			msg = "read done="+get();
		} catch ( InterruptedException e ) {
			e.printStackTrace();
		} catch ( ExecutionException e ) {
			e.printStackTrace();
		}
		SimApp.displayText("<rcvd="+msg+">");
	}
}
