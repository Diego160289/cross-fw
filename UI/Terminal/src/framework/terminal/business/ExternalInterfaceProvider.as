package framework.terminal.business
{
	import flash.external.ExternalInterface;
	
	import framework.terminal.model.ApplicationModelLocator;
	import framework.terminal.utils.common.Serialize;
	
	import mx.controls.Alert;
	
	public class ExternalInterfaceProvider
	{
		private static var instance : ExternalInterfaceProvider;
		//
		public function ExternalInterfaceProvider(enforcer : SingletonEnforcer)
		{
			if (enforcer == null)
			{
				throw new Error("You can have only one instance of ExternalInterfaceServiceProvider.");
			}
			//
			if (!ExternalInterface.available)
			{
				throw new Error("ExternalInterface is not available. Flash player 8 or higher required.");
			}
			//
			ExternalInterface.marshallExceptions = true;
		}
			
		//
		public static function getInstance() : ExternalInterfaceProvider
		{
			if (instance == null)
			{
				instance = new ExternalInterfaceProvider(new SingletonEnforcer());
			}
			//
			return instance;
		}
		
		public function registerContext(context : CallbackContext) : void
		{
			try
			{
				ExternalInterface.addCallback(context.callbackName, context.responder);
			}
			catch (e : Error)
			{
				ShowErrorMessage(e);
			}
		}
		
		public function invoke(delegate : CallbackDelegate) : void
		{
			var data : Object = new Object();
			//
			if (delegate.data)
			{
				data.params = Serialize.cloneObject(delegate.data);
			}
			
			data.method		= delegate.context.methodName;
			data.callback	= delegate.context.callbackName;
			data.serviceId	= delegate.context.serviceId;
			data.frameId 	= delegate.context.frameId;
			data.objectId 	= delegate.context.objectId;
			
			data.Invoker = delegate.context.methodName;
			data.Callback = delegate.context.callbackName;
			 
			//
			try
			{
				ExternalInterface.call(delegate.context.methodName, data);
			}
			catch (e : Error)
			{
				ShowErrorMessage(e);
			}
		}
		
		private function ShowErrorMessage(info : Error) : void
		{
			Alert.show(info.message, "ExternalInterface Error");
		}
	}
}

class SingletonEnforcer
{
}

<!--
<?xml version='1.0' encoding='utf-8'?>
<invoke name="GetGroups" returntype="xml">
  <arguments>
    <object>
      <property id="method">
        <string>имя вызываемого метода на стороне C++</string>
      </property>
      <property id="callback">
        <string>имя flex метода подлежащего вызову со стороны С++</string>
      </property>
      <property id="serviceId">
        <string>сервис С++ предназначенный предоставляющий данные</string>
      </property>
      <property id="frameId">
        <string>имя флейма с которого инициирован вызов C++</string>
      </property>
      <property id="objectId">
        <string>имя компонента UI с которого инициирован вызов C++</string>
      </property>
      <property id="params">
        <object>
          <property id="MyPrm1"> 
            <string>1</string>
          </property>
          <property id="MyPrm2">
            <string>qwerty</string>
          </property>
        </object>
      </property>
    </object>
  </arguments>
</invoke>
-->
