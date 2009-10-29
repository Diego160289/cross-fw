package framework.terminal.utils.math
{
/**
	  * Берем величину, распределенную равномерно (Math.random()) и переводим в величину, распределенную стандартно.
	  * В данном случае используется один из самых быстрых алгоритмов такого перевода - преобразование Бокса-Мюллера. 
	  */
	final public class Random
	{
		/**
		 * @param mean: среднее значение
		 * @param deviation: разброс (среднеквадратичное отклонение)
		 * @return Массив из двух элементов: [x, y]
		 */
		public static function gaussianRandomPolar(mean : Number, deviation : Number) : Array
		{
			var x1 : Number, x2 : Number;
			var y1 : Number, y2 : Number;
			var w : Number;
			
			// Преобразование Бокса—Мюллера			
			// Box-Muller transform			
			do {
				x1 = 2.0 * Math.random() - 1.0;
				x2 = 2.0 * Math.random() - 1.0;
				w = x1 * x1 + x2 * x2;
			} while ( w >= 1.0 );
		
			w = Math.sqrt((-2.0 * Math.log(w) ) / w);
		
			y1 = x1 * w * deviation + mean;
			y2 = x2 * w * deviation + mean;
			
			return [y1, y2];
		}

		/**
		 * @param mean: среднее значение
		 * @param deviation: разброс (среднеквадратичное отклонение)
		 * @return Число
		 */
		public static function gaussianRandom(mean : Number, deviation : Number) : Number
		{
			var point:Array = gaussianRandomPolar(mean, deviation);
			return Math.sqrt(point[0] * point[0] + point[1] * point[1]);
		}	
	}
}