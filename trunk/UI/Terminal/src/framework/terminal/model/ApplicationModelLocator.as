package framework.terminal.model
{
	import com.adobe.cairngorm.model.IModelLocator;
	
	import mx.collections.ArrayCollection;

	[Bindable]
	public class ApplicationModelLocator implements IModelLocator
	{
		private static var instance : ApplicationModelLocator;
		
		//
		public function ApplicationModelLocator(enforcer : SingletonEnforcer)
		{
			if (enforcer == null)
			{
				throw new Error("You can have only one instance of ApplicationModelLocator.");
			}
		}
		
		//
		public static function getInstance() : ApplicationModelLocator
		{
			if (instance == null)
			{
				instance = new ApplicationModelLocator(new SingletonEnforcer());
			}
			//
			return instance;
		}
		
		// DEFINE Constants 
		public static const BUSINESS_CATEGORIES_SCREEN	: uint = 0;
		public static const PROVIDERS_SCREEN			: uint = 1;
		//
		public static const ENTER_CELL_PHONE_NUMBER_SCREEN	: uint = 2;
		public static const VERIFY_CELL_PHONE_NUMBER_SCREEN	: uint = 3;
		public static const PAYMENT_SCREEN					: uint = 4;
		public static const THANK_YOU_SCREEN				: uint = 5;
		//
		public static const DATA_LOADING_EVENT	 : String = "DataLoadingEvent";
		public static const ITEM_CLICKED_EVENT	 : String = "ItemClickedEvent";
		public static const BUTTON_CLICKED_EVENT : String = "ButtonClickedEvent";
		//
		public static const VERSION			: String = "0.0.0.1";
		public static const PRODUCT_NAME	: String = "Terminal UI";
		public static const RELEASE_DATE	: String = "01.01.2009";

		// DEFINE Variables
		public var workflowState : uint = 0;
		//
		public var businessCategories	: ArrayCollection = new ArrayCollection();
		public var providers 			: ArrayCollection = new ArrayCollection();
		public var cellPhoneNumber		: String;
		public var amount				: String;
		//
		public var frameId 		: String;
		public var serviceId 	: String;
	}
}

class SingletonEnforcer 
{
}
