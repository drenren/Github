package market;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;


public class StockDBFeed extends StockDataFeed {
		private Connection connect = null;
		private ResultSet resultSet = null;
		private PreparedStatement preparedStatement = null;
				
		public StockDBFeed(String _host, String _user, String _pw, String _defaultdb) throws ClassNotFoundException, SQLException
		{
			Class.forName("com.mysql.jdbc.Driver");
			 String url = "jdbc:mysql://"+_host+":3306/"+_defaultdb;
			 connect = DriverManager.getConnection(url,_user,_pw);
			 
			 
			 
		}
		
		public void subscribe(String _ticker, Calendar _date) throws Exception
		{
			SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
			String sql = "select * from MktData.DowJonesMinuteHistory where Ticker = '"+_ticker+"' and Date ='"+format.format(new Date(_date.getTimeInMillis()))+"' ";
			
			preparedStatement=connect.prepareStatement(sql);

			
			resultSet = preparedStatement.executeQuery();
		}
		
		public Quote get_next_quote() throws Exception
		{
			while (resultSet.next())
			{
				Calendar cal = Calendar.getInstance();
				
				String strDateTime = resultSet.getString("Date")+" "+resultSet.getString("Time");
				
				Date d = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").parse(strDateTime);
				cal.setTime(d);
				
				return new Quote( cal, 
								 resultSet.getString("Ticker"),
								 resultSet.getDouble("Open"),
								 resultSet.getDouble("Close"),
								 resultSet.getDouble("High"),
								 resultSet.getDouble("Low"),
								 resultSet.getInt("Volumne"));
			}
			return null;
		} 
		
 
}