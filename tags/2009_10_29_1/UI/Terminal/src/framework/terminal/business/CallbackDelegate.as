package framework.terminal.business
{
	import mx.rpc.IResponder;
	
	public class CallbackDelegate
	{
		public var context : CallbackContext;
		public var data : Object;
		
		public function CallbackDelegate(pContext : CallbackContext, pData : Object = null)
		{
			this.context = pContext;
			this.data = pData;
		}
	}
} 