using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;
using System.Net;
using System.IO.Ports;

namespace CricketScores
{
    class Program
    {
        static void Main(string[] args)
        {
            Regex country = new Regex("<title>.*Australia.*</title>");
            Regex wickets = new Regex("/(?<wickets>[0-9]{1,2})");

            using (SerialPort serial = new SerialPort("COM5", 38400))
            {
                serial.Open();

                string prevWickets = string.Empty;

                using (WebClient wc = new WebClient())
                {
                    while (true)
                    {
                        string html = wc.DownloadString("http://static.cricinfo.com/rss/livescores.xml");

                        var countryMatch = country.Match(html);
                        if (countryMatch.Success)
                        {
                            var wicketsMatch = wickets.Matches(countryMatch.Value);
                            string wicketsStr = string.Join(",", wicketsMatch.Cast<Match>().Select(m => m.Groups[1].Value));
                            Console.WriteLine(wicketsStr);

                            if (serial.IsOpen && wicketsStr != prevWickets)
                            {
                                prevWickets = wicketsStr;
                                serial.Write(new byte[] { 0x77 }, 0, 1);
                            }
                        }

                        System.Threading.Thread.Sleep(60 * 1000);
                    }
                }
            }
        }
    }
}
