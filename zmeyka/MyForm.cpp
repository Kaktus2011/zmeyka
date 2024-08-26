#include "MyForm.h"
using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
void main(array<String^>^ args) {
	//Базовые настройки для запуска формы
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	//Запускаем указанную форму
	zmeyka::MyForm form;
	Application::Run(% form);
}
struct Vector2
{
	int X, Y;
};
Vector2 direction;
Vector2 pF;
Vector2 gameArena;
zmeyka::MyForm::MyForm(void)
{
	InitializeComponent();
	gameArena.X = 500;
	gameArena.Y = 500;
	firstLaunch = true;
	NewGame();
}
void zmeyka::MyForm::ChackBorders()
{
	if (Serpens[0]->Location.X >= pictureBox3->Location.X || Serpens[0]->
		Location.X <= pictureBox2->Location.X) {
		GameOver();
	}

	if (Serpens[0]->Location.Y <= pictureBox1->Location.Y || Serpens[0]->
		Location.Y >= pictureBox4->Location.Y) {
		GameOver();
	}

	label2->Text = "Кооринаты змейки: (" + Convert::ToString(Serpens[0]->
		Location.X) +
		", " + Convert::ToString(Serpens[0]->Location.Y) + ")";
}
System::Void zmeyka::MyForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
{   
	uI = Convert::ToSingle(numericUpDown1->Value);
	timer->Interval = uI;
	button1->Enabled = false;
	numericUpDown1->Enabled = false;
	groupBox2->Visible = false;
	Play = true;
	timer->Start();
	return System::Void();
}

System::Void zmeyka::MyForm::новаяИграToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	NewGame();
	return System::Void();
}

System::Void zmeyka::MyForm::паузапродолжитьToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (Play)
	{
		Play = false;
	}
	else
		Play = true;
	return System::Void();
}

System::Void zmeyka::MyForm::настройкиToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	if(groupBox2->Visible==false)
	{
		Play = false;
		button1->Enabled = true;
		numericUpDown1->Enabled = true;
		groupBox2->Visible = true;
	}
	else
	{
		Play = true;
		timer->Start();
		button1->Enabled = false;
		numericUpDown1->Enabled = false;
		groupBox2->Visible = false;
	}
	return System::Void();
}

System::Void zmeyka::MyForm::информацияToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	MessageBox::Show("правила\n"
		"1. для управления не используйте стрелки;)\n"
		"2. не ешьте фрукты, они убеньшают ваш счёт т уменьшают змейку;)\n"
		"3. лучше ешьте себя и косайтесь стенок;)\n");
	return System::Void();
}

System::Void zmeyka::MyForm::выходToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	Application::Exit();
}

System::Void zmeyka::MyForm::MyForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
{
	if (e->KeyCode.ToString() == "D")
	{
		direction.X = 1;
		direction.Y = 0;
	}
	else if (e->KeyCode.ToString() == "A")
	{
		direction.X = -1;
		direction.Y = 0;
	}
	else if (e->KeyCode.ToString() == "W")
	{
		direction.X = 0;
		direction.Y = -1;
	}
	else if (e->KeyCode.ToString() == "S")
	{
		direction.X = 0;
		direction.Y = 1;
	}
	return System::Void();
}

System::Void zmeyka::MyForm::таймер_Tick(System::Object^ sender, System::EventArgs^ e)
{
	if (!Die && Play)
	{
		Movement();
		Eating();
		EatYourself();
		ChackBorders();
	}
	else if (Die && Play)
	{
		timer->Stop();
		MessageBox::Show("игра окончена", "перезапустите игру");
	}
	else if (!Die && !Play)
	{
		timer->Stop();
		MessageBox::Show("игра оcтоновлена", "возобновите игру");
	}
	return System::Void();
}

void zmeyka::MyForm::GeneratePositionFruit()
{
	Random^ rand = gcnew Random();
	pF.X = rand->Next(33, gameArena.X - 70);
	pF.Y = rand->Next(180, gameArena.Y - 70);
	for (int i = 0; i<score; i++)
	{
		if (pF.X == Serpens[i]->Location.X && pF.Y == Serpens[i]->Location.Y)
		{
			GeneratePositionFruit();
		}
	}
	int tempX = pF.X % step;
	pF.X -= tempX;
	int tempY = pF.Y % step;
	pF.Y -= tempY;
	fruit->Location = Point(pF.X, pF.Y);
	this->Controls->Add(fruit);
}
void zmeyka::MyForm::Eating()
{
	if (Serpens[0]->Location.X == pF.X && Serpens[0]->Location.Y == pF.Y)
	{
		label1->Text = "Счёт: " + ++score;
		Serpens[score] = gcnew PictureBox();
		Serpens[score]->Location = Point(Serpens[score - 1]->Location.X + step * direction.X, Serpens[score - 1]->Location.Y + step * direction.Y);
		Serpens[score]->BackColor = Color::Green;
		Serpens[score]->Width = step;
		Serpens[score]->Height = step;
		this->Controls->Add(Serpens[score]);
		GeneratePositionFruit();
	}
}
void zmeyka::MyForm::Movement()
{
	for (int i = score;i >= 1; i--)
	{
		Serpens[i]->Location = Serpens[i - 1]->Location;
	}
	Serpens[0]->Location = Point(Serpens[0]->Location.X + direction.X * step, Serpens[0]->Location.Y + direction.Y * step);
}
void zmeyka::MyForm::EatYourself()
{
	for (int i = 1;i < score;i++)
	{
		if (Serpens[0]->Location == Serpens[i]->Location)
		{
			GameOver();
		}
	}
}
void zmeyka::MyForm::GameOver()
{
	Play = true;
	Die = true;
	label4->Visible = true;
}
void zmeyka::MyForm::NewGame()
{
	if (!firstLaunch)
	{
		this->Controls->Remove(fruit);
		for (int i = 0;i <= score;i++)
		{
			this->Controls->Remove(Serpens[i]);
		}
		score = 0;
	}
	else
		firstLaunch = false;
	Serpens = gcnew array <PictureBox^, 1>(400);
	Serpens[0] = gcnew PictureBox();
	Serpens[0]->Location = Point(200, 200);
	Serpens[0]->BackColor=Color::Olive;
	Serpens[0]->Width = step;
	Serpens[0]->Height = step;
	score = 0;
	this->Controls->Add(Serpens[0]);
	fruit = gcnew PictureBox();
	fruit->BackColor = Color::Red;
	fruit->Width = step;
	fruit->Height = step;
	GeneratePositionFruit();
	timer->Interval = uI;
	timer->Start();
	direction.X = 1;
	direction.Y = 0;
	Play = true;
	Die = false;
	label1->Text = "Счёт: ";
	label4->Visible = false;
	groupBox2->Visible = false;
	label2->Text = "координаты(" + Convert::ToString(Serpens[0]->Location.X) + "," + Convert::ToString(Serpens[0]->Location.Y) + ")";

}