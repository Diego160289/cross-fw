package framework.terminal.components.custom.preloaders
{
	import flash.display.MovieClip;
    import flash.display.Sprite;
    import flash.events.Event;
    import flash.events.ProgressEvent;
    import flash.events.TimerEvent;
    import flash.utils.Timer;
    
    import mx.events.FlexEvent;
    import mx.preloaders.DownloadProgressBar;
    
    public class CustomAppPreloader extends DownloadProgressBar
    {
        // ----------------------------------------
        //  Private Variables
        // ----------------------------------------
        
        /**
        * @private
        * @property Embed the preloader MovieClip from your .swf file.
        */
        [Embed(source="assets/swf/MainPreloader.swf", symbol="Preloader")]
        private var PreloaderSymbol:Class;
        
        /**
        * @private
        * @property Reference to your preloader MovieClip.
        */
        private var _preloaderSymbol:MovieClip;
        
        /**
        * @private
        * @property Reference to the Preloader Class. We'll be using it to
        * listen for certain events and so that we can later remove
        * these listeners when we don't need them anymore.
        */
        private var _preloader:Sprite;
        
        /**
        * @private
        * @property Will be used later on to add an additional delay to the
        * preloader "outAnimation" so that the applications content won't
        * exactly after the animation has finished.
        */
        private var _timer:Timer;
        
        /**
        * @private
        * @default 500
        * @property The delay amount that we'll add to the end of our animation.
        */
        private var _timerDelay:int = 500;
        
        // ----------------------------------------
        //  CONSTRUCTOR
        // ----------------------------------------
        
        public function CustomAppPreloader()
        {
            super();
            
            // Instantiate and add the Preloader MovieClip to the main application;
            // don't forget to call stop(); because any code that was added to your
            // MovieClip in Flash will be ignored once embedded into Flex
            _preloaderSymbol = new PreloaderSymbol() as MovieClip;
            this.addChild(_preloaderSymbol);
            _preloaderSymbol.stop();
        }
        
        // ----------------------------------------
        //  Overridden Methods
        // ----------------------------------------
        
        /**
        * @public
        * The Preloader class passes in a reference to itself to the display 
        * class so that it can listen for events from the preloader.
        */
        override public function set preloader(preloader:Sprite):void
        {
            _preloader = preloader;
            
            _preloader.addEventListener(ProgressEvent.PROGRESS, onDownloadProgress);
            _preloader.addEventListener(FlexEvent.INIT_COMPLETE, onFlexInitComplete);
            //
			this.x = this.stageWidth >> 1;
            this.y = this.stageHeight >> 1;
        }
        
        // ----------------------------------------
        //  Private Methods
        // ----------------------------------------
        
        /**
        * @private
        * Responsible for updating the preloader while data is being downloaded.
        * We calculate the amount loaded and transform the value into percentage that
        * we can use together with the gotoAndStop() method to step between the
        * preloader frames (updating the preloader this way).
        */
        private function onDownloadProgress(event:ProgressEvent):void
        {
            var percent:Number = (100 * (event.bytesLoaded / event.bytesTotal)) >> 0;
            _preloaderSymbol.gotoAndStop(percent);
        }
        
        /**
        * @private
        * Is triggered when Flex is ready read to run the application. Before actually
        * displaying the main application, we'll want to animate out our preloader
        * and this is where we trigger this out animation.
        */
        private function onFlexInitComplete(event:FlexEvent):void
        {
            var frameNumber:int = _preloaderSymbol.totalFrames - 1;
            
            // With the help of the addFrameScript() method we can assign actions to
            // a certain frame of our preloader dynamically. For this preloader we'll
            // assing the onAnimationFinish() method to the frame before the last frame
            // of the preloader so that the method can be triggered once it reaches the
            // last frame. If we'd assign it to the last frame, then the method will
            // never trigger and the preloader will go into a continuous loop.
            _preloaderSymbol.addFrameScript(frameNumber, onAnimationFinish);
            _preloaderSymbol.gotoAndPlay("outAnimation");
        }
        
        /**
        * @private
        * We stop the out animation so that the preloader won't loop and with the
        * help of our Timer object we can add that additional delay to the end of the
        * "outAnimation" so that the application content won't show immediately.
        */
        private function onAnimationFinish():void
        {
            _preloaderSymbol.stop();
            
            _timer = new Timer(_timerDelay, 0);
            _timer.addEventListener(TimerEvent.TIMER, onTimer);
            _timer.start();
        }
        
        /**
        * @private
        * This method is triggered by our Timer object. It's is responsible for
        * removing the preloader from the display list and calling two important
        * methods that will remove all the event listeners we added to our objects
        * during the loading proccess and remove the references to the objects
        * we won't be using from this point. Finally, it dispatches a COMPLETE
        * event so that the application will know that the loading has ended.
        */
        private function onTimer(event:TimerEvent):void
        {
            _timer.stop();
            this.removeChild(_preloaderSymbol);
            
            killEventListeners();
            killObjects();
            
            dispatchEvent(new Event(Event.COMPLETE));
        }
        
        /**
        * @private
        * Responsible for removing the event listeners that we have added during
        * the application loading process.
        */
        private function killEventListeners():void
        {
            _preloader.removeEventListener(ProgressEvent.PROGRESS, onDownloadProgress);
            _preloader.removeEventListener(FlexEvent.INIT_COMPLETE, onFlexInitComplete);
            _timer.removeEventListener(TimerEvent.TIMER, onTimer);
        }
        
        /**
        * @private
        * Remove the references to the objects we won't be using from this point on,
        * making our objects more legible to Garbage Collection this way.
        */
        private function killObjects():void
        {
            _timer = null;
            _preloader = null;
            _preloaderSymbol = null;
            PreloaderSymbol = null;
        }
    }
}