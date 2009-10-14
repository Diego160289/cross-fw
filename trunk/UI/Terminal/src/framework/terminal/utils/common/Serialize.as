package framework.terminal.utils.common
{
	import flash.utils.ByteArray;

	final public class Serialize
	{
	    public static function cloneObject(o : Object) : Object
	    {
	       var bytes:ByteArray = new ByteArray();
	       bytes.writeObject(o);
	       bytes.position = 0;
	       //
	       return bytes.readObject();
	    }
	}
}