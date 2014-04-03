package market;

import java.util.Calendar;

public class Quote {
		final public double open;
		final public double close;
		final public double high;
		final public double low;
		final public int vol;
		final public Calendar date;
		final public String ticker;
		
		Quote(Calendar _date, String _ticker, double _open, double _close, double _high, double _low, int _vol )
		{
			this.date=_date;
			this.ticker=_ticker;
			this.open = _open;
			this.close= _close;
			this.high = _high;
			this.low = _low;
			this.vol = _vol;
		}
		public String toString()
		{
			return new String(date.toString() +"\t" + open + "\t");
		}
};