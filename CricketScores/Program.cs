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
            ////Regex country = new Regex("<title>.*India.*</title>");
            Regex wicketsRuns = new Regex(@"(?<runs>[0-9]{1,3})/(?<wickets>[0-9]{1,2}) \*|(?<runs>[0-9]{1,3}) \*");
            Regex teamsRegex = new Regex(@"(?<team1>[A-Za-z ]*) (?<score1>[0-9\/]*).*v (?<team2>[A-Za-z ]*) *(?<score2>[0-9\/]*)");

            using (SerialPort serial = new SerialPort("COM8", 38400))
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
                            var wicketsRunsMatchs = wicketsRuns.Matches(countryMatch.Value);

                            var wrm = wicketsRuns.Matches(countryMatch.Value).Cast<Match>();

                            string wicketsStr = string.Empty;
                            string runsStr = string.Empty;

                            if (wrm.Any())
                            {
                                var wicketsRunsMatch = wrm.Last();
                                wicketsStr = wicketsRunsMatch.Groups["wickets"].Value;
                                runsStr = wicketsRunsMatch.Groups["runs"].Value;
                            }

                            Console.WriteLine(countryMatch.Value);
                            var teams = teamsRegex.Matches(countryMatch.Value);

                            string team1 = Truncate(teams[0].Groups["team1"].Value.Trim(), 29);
                            string team2 = Truncate(teams[0].Groups["team2"].Value.Trim(), 29);

                            string score1 = teams[0].Groups["score1"].Value.Trim().PadLeft(7);
                            string score2 = teams[0].Groups["score2"].Value.Trim().PadLeft(7);

                            serial.Write("1t" + team1 + "\n");
                            serial.Write("2t" + team2 + "\n");

                            serial.Write("1s" + score1 + "\n");
                            serial.Write("2s" + score2 + "\n"); 

                            if (string.IsNullOrWhiteSpace(wicketsStr))
                            {
                                Console.WriteLine(countryMatch.Value);

                                if (string.IsNullOrWhiteSpace(wicketsStr))
                                {
                                    wicketsStr = "0";
                                }

                                Console.WriteLine("r" + runsStr);
                                serial.Write("r" + runsStr + "\n");

                                Console.WriteLine("w" + wicketsStr);
                                serial.Write("w" + wicketsStr + "\n");
                            }
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
