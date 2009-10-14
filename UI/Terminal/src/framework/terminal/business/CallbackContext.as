package framework.terminal.business
{
	import mx.rpc.IResponder;
	
	public class CallbackContext
	{
			public var methodName	: String;
			public var callbackName : String;
			public var responder 	: Function
			public var objectId 	: String;
			public var frameId 		: String;
			public var serviceId 	: String;

			public function CallbackContext(pCallbackName 	: String, 
											pResponder 		: Function, 
											pMethodName 	: String = null,
											pObjectId 		: String = null,
											pFrameId 		: String = null,
											pServiceId 		: String = null) 
			{
				this.methodName		= pMethodName;
				this.callbackName	= pCallbackName;
				this.responder		= pResponder;
				this.objectId		= pObjectId;
				this.frameId		= pFrameId;
				this.serviceId		= pServiceId;
			}
	}
}