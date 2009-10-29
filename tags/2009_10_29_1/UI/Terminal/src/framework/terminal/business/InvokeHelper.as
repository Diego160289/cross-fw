package framework.terminal.business
{
	public class InvokeHelper
	{
		private static var serviceProvider : ExternalInterfaceProvider = ExternalInterfaceProvider.getInstance();

		public static function sendAndForget(pMethodName 	: String, 
											 pData 			: Object, 
											 pObjectId 		: String, 
											 pFrameId 		: String, 
											 pServiceId 	: String) : void
		{
				var context : CallbackContext = new CallbackContext
													(
														"", 
														null,
														pMethodName, 
														pObjectId, 
														pFrameId,
														pServiceId
													);
				//
				serviceProvider.invoke(new CallbackDelegate(context, pData));
		}
		//
		public static function sendAndLoad(pMethodName	 : String,
										   pResponder	 : Function,
										   pData 		 : Object,
										   pObjectId 	 : String,
										   pFrameId 	 : String,
										   pServiceId 	 : String) : void
		{
				var context : CallbackContext = new CallbackContext
													(
														"On" + pMethodName, 
														pResponder, 
														pMethodName,
														pObjectId, 
														pFrameId,
														pServiceId
													);
				
				serviceProvider.registerContext(context); 
				//
				serviceProvider.invoke(new CallbackDelegate(context));
		}
	}
}