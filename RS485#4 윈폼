using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace example74_2
{
    public partial class Form1 : Form
    {
        bool is_received = false;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string[] port = SerialPort.GetPortNames();

            for(int i = 0; i < port.Length; i++)
            {
                comboBox1.Items.Add(port[i]);
            }
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
            {
                //시리얼포트가 닫혀있다면~
                if(comboBox1.SelectedIndex != -1)
                {
                    serialPort1.PortName = comboBox1.SelectedItem.ToString();
                    serialPort1.Open();

                    richTextBox1.Text += "시리얼포트가 열렸습니다!\n";
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedItems.Count == 1 && textBox10.Text.Length <= 4 && textBox10.Text.Length > 0)
            {
                //1~4
                string num = textBox10.Text;
                while(num.Length < 4)
                {
                    num = "0" + num;
                }
                richTextBox1.Text += num + "\n";
                byte[] send = new byte[10];
                send[0] = byte.Parse(listView1.SelectedItems[0].Text);
                send[1] = 0x00;
                send[2] = (byte)num[0];
                send[3] = (byte)num[1];
                send[4] = (byte)num[2];
                send[5] = (byte)num[3];
                send[6] = byte.Parse(textBox6.Text);
                send[7] = byte.Parse(textBox7.Text);
                send[8] = byte.Parse(textBox8.Text);
                send[9] = byte.Parse(textBox9.Text);

                //request!
                is_received = false;
                serialPort1.Write(send, 0, 10);
                richTextBox1.Text += "Request!\n";

                //시간을 측정해보자!
                DateTime timeout = DateTime.Now;
                while (true)
                {
                    //무한루프생성
                    if ((DateTime.Now - timeout).TotalMilliseconds > 1000)
                    {
                        richTextBox1.Text += "TIMEOUT\n";
                        listView1.SelectedItems[0].SubItems[1].Text = "FALSE";

                        break;
                    }
                    if (is_received)
                    {
                        //richTextBox1.Text += "RECEIVED!\n";
                        listView1.SelectedItems[0].SubItems[1].Text = "TRUE";
                        byte[] recv = new byte[10];
                        serialPort1.Read(recv, 0, 10);

                        //리스트뷰에 수신메시지를 집어넣자!
                        ListViewItem lvi = new ListViewItem();
                        lvi.Text = recv[0].ToString("X");
                        for (int i = 1; i < 10; i++)
                        {
                            lvi.SubItems.Add(recv[i].ToString("X"));
                        }

                        listView2.Items.Add(lvi);

                        break;
                    }
                }
            }
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            is_received = true;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if(textBox1.Text != "")
            {
                ListViewItem lvi = new ListViewItem();
                lvi.Text = textBox1.Text;
                lvi.SubItems.Add("-");

                listView1.Items.Add(lvi);
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if(listView1.SelectedItems.Count == 1)
            {
                //richTextBox1.Text = listView1.SelectedIndices[0].ToString();
                listView1.Items.RemoveAt(listView1.SelectedIndices[0]);
            }
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedItems.Count == 1)
            {
                byte[] send = new byte[10];
                send[0] = byte.Parse(listView1.SelectedItems[0].Text);
                send[1] = 0x00;
                send[2] = byte.Parse(textBox2.Text);
                send[3] = byte.Parse(textBox3.Text);
                send[4] = byte.Parse(textBox4.Text);
                send[5] = byte.Parse(textBox5.Text);
                send[6] = byte.Parse(textBox6.Text);
                send[7] = byte.Parse(textBox7.Text);
                send[8] = byte.Parse(textBox8.Text);
                send[9] = byte.Parse(textBox9.Text);

                //request!
                is_received = false;
                serialPort1.Write(send, 0, 10);
                richTextBox1.Text += "Request!\n";

                //시간을 측정해보자!
                DateTime timeout = DateTime.Now;
                while (true)
                {
                    //무한루프생성
                    if ((DateTime.Now - timeout).TotalMilliseconds > 1000)
                    {
                        richTextBox1.Text += "TIMEOUT\n";
                        listView1.SelectedItems[0].SubItems[1].Text = "FALSE";
                        
                        break;
                    }
                    if (is_received)
                    {
                        //richTextBox1.Text += "RECEIVED!\n";
                        listView1.SelectedItems[0].SubItems[1].Text = "TRUE";
                        byte[] recv = new byte[10];
                        serialPort1.Read(recv, 0, 10);

                        //리스트뷰에 수신메시지를 집어넣자!
                        ListViewItem lvi = new ListViewItem();
                        lvi.Text = recv[0].ToString("X");
                        for(int i = 1; i < 10; i++)
                        {
                            lvi.SubItems.Add(recv[i].ToString("X"));
                        }

                        listView2.Items.Add(lvi);

                        break;
                    }
                }
            }
        }

        private void button6_Click(object sender, EventArgs e)
        {
            
            if(textBox11.Text.Length <= 4 && textBox11.Text.Length > 0)
            {
                //1~4
                string num = textBox11.Text;
                while (num.Length < 4)
                {
                    num = "0" + num;
                }

                for (int j = 0; j < listView1.Items.Count; j++)
                {
                    
                    //richTextBox1.Text += num + "\n";
                    byte[] send = new byte[10];
                    send[0] = byte.Parse(listView1.Items[j].SubItems[0].Text);
                    send[1] = 0x00;
                    send[2] = (byte)num[0];
                    send[3] = (byte)num[1];
                    send[4] = (byte)num[2];
                    send[5] = (byte)num[3];
                    send[6] = byte.Parse(textBox6.Text);
                    send[7] = byte.Parse(textBox7.Text);
                    send[8] = byte.Parse(textBox8.Text);
                    send[9] = byte.Parse(textBox9.Text);

                    //request!
                    is_received = false;
                    serialPort1.Write(send, 0, 10);
                    richTextBox1.Text += "Request!\n";

                    //시간을 측정해보자!
                    DateTime timeout = DateTime.Now;
                    while (true)
                    {
                        //무한루프생성
                        if ((DateTime.Now - timeout).TotalMilliseconds > 1000)
                        {
                            richTextBox1.Text += "TIMEOUT\n";
                            listView1.Items[j].SubItems[1].Text = "FALSE";

                            break;
                        }
                        if (is_received)
                        {
                            //richTextBox1.Text += "RECEIVED!\n";
                            listView1.Items[j].SubItems[1].Text = "TRUE";
                            byte[] recv = new byte[10];
                            serialPort1.Read(recv, 0, 10);

                            //리스트뷰에 수신메시지를 집어넣자!
                            ListViewItem lvi = new ListViewItem();
                            lvi.Text = recv[0].ToString("X");
                            for (int i = 1; i < 10; i++)
                            {
                                lvi.SubItems.Add(recv[i].ToString("X"));
                            }

                            listView2.Items.Add(lvi);

                            break;
                        }
                    }
                }
            }
        }
    }
}
