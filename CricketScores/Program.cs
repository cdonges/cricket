//-----------------------------------------------------------------------
// <copyright file="Program.cs" company="gen3 media">
//     Copyright chris donges
// </copyright>
//-----------------------------------------------------------------------

namespace CricketScores
{
    using System;
    using System.Collections.Generic;
    using System.IO.Ports;
    using System.Linq;
    using System.Net;
    using System.Text;
    using System.Text.RegularExpressions;
    using System.Threading.Tasks;
        
    /// <summary>
    /// Main program
    /// </summary>
    public class Program
    {
        /// <summary>
        /// Main program
        /// </summary>
        /// <param name="args">passed arguments</param>
        public static void Main(string[] args)
        {
            // aussie aussie aussie
            Regex country = new Regex("<title>.*Australia.*</title>");
            Regex wicketsRuns = new Regex(@"(?<runs>[0-9]{1,3})/(?<wickets>[0-9]{1,2}) \*|(?<runs>[0-9]{1,3}) \*");
            Regex teamsRegex = new Regex(@"<title>(?<team1>.*)v(?<team2>.*)<\/title>");

            using (SerialPort serial = new SerialPort("COM3", 38400))
            {
                serial.Encoding = System.Text.Encoding.ASCII;
                serial.Open();

                using (WebClient wc = new WebClient())
                {
                    while (true)
                    {
                        string html = wc.DownloadString("http://static.cricinfo.com/rss/livescores.xml");

                        var countryMatch = country.Match(html);
                        if (countryMatch.Success)
                        {
                            var wicketsRunsMatch = wicketsRuns.Matches(countryMatch.Value).Cast<Match>().Last();
                            string wicketsStr = wicketsRunsMatch.Groups["wickets"].Value;
                            string runsStr = wicketsRunsMatch.Groups["runs"].Value;

                            Console.WriteLine(countryMatch.Value);
                            var teams = teamsRegex.Matches(countryMatch.Value);

                            string team1 = Truncate(teams[0].Groups["team1"].Value.Trim(), 16).PadRight(16);
                            string team2 = Truncate(teams[0].Groups["team2"].Value.Trim(), 16).PadRight(16);

                            serial.Write("1" + team1 + "\n");
                            serial.Write("2" + team2 + "\n"); 

                            if (string.IsNullOrWhiteSpace(wicketsStr))
                            {
                                wicketsStr = "0";
                            }

                            Console.WriteLine("w" + wicketsStr);
                            serial.Write("w" + wicketsStr + "\n"); 
                            Console.WriteLine("r" + runsStr);
                            serial.Write("r" + runsStr + "\n");
                        }
                        else
                        {
                            Console.WriteLine("No Australia matches");
                        }

                        // try every minute
                        System.Threading.Thread.Sleep(60 * 1000);
                    }
                }
            }
        }

        public static string Truncate(string str, int maxLength)
        {
            return str.Substring(0, Math.Min(str.Length, maxLength));
        }
    }
}
