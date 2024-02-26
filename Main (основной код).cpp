//---------------------------------------------------------------------------
#include <vcl.h>
#include <IniFiles.hpp>
#include <SysUtils.hpp>



#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "pies"
#pragma resource "*.dfm"
TForm1 *Form1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	LoadDataFromConfig();

	if(rootFolderPath.Length() == 0){
		TabControl5->Visible = true;
	}else{
		TabControl5->Visible = false;
		RootFolderPathEdit1->Text = rootFolderPath;
		RootFolderPathEdit2->Text = rootFolderPath;
	}


	// Loading data on starting app
	LoadDataOfDay();
	LoadSubjectsData(Now());
	LoadTasksData(Now());
	LoadDaysData(Now());
}

//TIMER LOGIC////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __fastcall TForm1::StartBtnClick(TObject *Sender)
{
	if ((TimeComboBox->ItemIndex != -1) && (SubjectsComboBox->ItemIndex != -1)) {
		Timer1 -> Enabled = !(Timer1 -> Enabled);
		if(Timer1 -> Enabled){
			timerCurrentSubject = SubjectsComboBox->Text;

			StartBtn -> Caption = "Reset";
		}else{
			Timer1 -> Enabled = false;
			StartBtn -> Caption = "Start";

			TStringList *timeParts = new TStringList();
			timeParts->Delimiter = ':';
			timeParts->DelimitedText = TimeComboBox->Text;
			if (TimeComboBox->ItemIndex != -1) {
				H = StrToInt(timeParts->Strings[0]);
				M = StrToInt(timeParts->Strings[1]);
				S = StrToInt(timeParts->Strings[2]);
			}else{
				H = 0;
				M = 0;
				S = 0;
			}
			TimeText -> Caption = formatTime(H, M, S);
		}
	} else {
		// Ни один элемент не выбран
		ShowMessage("Choose subject and time");
	}

}

//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	if (H == 0 && M == 0 && S == 0)
	{
		Timer1->Enabled = false;
		StartBtn -> Caption = "Start"; ///////////////////////////////////////////

		bool noSubjectInConfig = true;
		for(int i = 0; i < subjectsNames.size(); i++){
			if(subjectsNames[i] == timerCurrentSubject){
				noSubjectInConfig = false;
				subjectsIntervals[i] = IntToStr(StrToInt(subjectsIntervals[i]) + StrToInt(timeStringToMinutes(TimeComboBox->Text)));
                SubjectsList->Cells[0][i] = subjectsNames[i];
				SubjectsList->Cells[1][i] = subjectsIntervals[i];
                break;
			}
		}

		if(noSubjectInConfig){//////////////////////////////////////////////////
			subjectsNames.push_back(timerCurrentSubject);
			subjectsIntervals.push_back(timeStringToMinutes(TimeComboBox->Text));
		}
		WriteArrayToConfig(subjectsConfigSection, "name",     subjectsNames);
		WriteArrayToConfig(subjectsConfigSection, "interval", subjectsIntervals);

		ReloadSubjectsData(Now());

        completedSubjectsNames.push_back(timerCurrentSubject);
		completedSubjectsTime.push_back(timeStringToMinutes(TimeComboBox->Text));
		for (int i = 0; i < completedSubjectsNames.size(); i++){
			ToDoList->Cells[0][i] = completedSubjectsNames[i];
			ToDoList->Cells[1][i] = ConvertMinutesToTime(StrToInt(completedSubjectsTime[i]));
		}

		completedTasksCounter++;

		dayTime += timeStringToMinutes(TimeComboBox->Text);
		TotalTimeText->Caption = formatTotalTime(StrToInt(dayTime));
		TDateTime currentDate = Date();
		System::Word year;
		System::Word month;
		System::Word day;
		DecodeDate(currentDate, year, month, day);


		WriteArrayToConfig(IntToStr(day) + L"." + IntToStr(month) + L"." + IntToStr(year), IntToStr(day) + L"." + IntToStr(month) + L"." + IntToStr(year) + L"_task", completedSubjectsNames);
		WriteArrayToConfig(IntToStr(day) + L"." + IntToStr(month) + L"." + IntToStr(year), IntToStr(day) + L"." + IntToStr(month) + L"." + IntToStr(year) + L"_time", completedSubjectsTime);
		SaveDataToConfig(IntToStr(month) + L"." + IntToStr(year), L"day_" + IntToStr(day - 1), dayTime);


		ReloadDaysData(Now());
	}
	if (S == 0 && M > 0)
	{
		M--;
		S = 60;
	}else if(S == 0 && M == 0 && H != 0){
		 M = 59;
		 S = 60;
		 H--;
	}

	if (M == 0 && H > 0)
	{
		H--;
		M = 59;
	}

	if (!(H == 0 && M == 0 && S == 0))
	{
		S--;
	}

	TimeText -> Caption = formatTime(H, M, S);

}
//END TIMER LOGIC////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __fastcall TForm1::MenuDayClick(TObject *Sender)
{
	TabControl1->Visible = true;
	TabControl2->Visible = false;
	TabControl3->Visible = false;
	TabControl4->Visible = false;
	TabControl5->Visible = false;

	LoadDataOfDay();////////////////////????????
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuDaysClick(TObject *Sender)
{
	TabControl1->Visible = false;
	TabControl2->Visible = true;
	TabControl3->Visible = false;
	TabControl4->Visible = false;
	TabControl5->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuProfileClick(TObject *Sender)
{
	TabControl1->Visible = false;
	TabControl2->Visible = false;
	TabControl3->Visible = true;
	TabControl4->Visible = false;
	TabControl5->Visible = false;
}
//---------------------------------------------------------------------------

 void __fastcall TForm1::MenuSchedulesClick(TObject *Sender)
{
	TabControl1->Visible = false;
	TabControl2->Visible = false;
	TabControl3->Visible = false;
	TabControl4->Visible = true;
	TabControl5->Visible = false;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::RootFolderBtnClick(TObject *Sender)
{
	TOpenDialog *OpenDialog1 = new TOpenDialog(this);

	// Настройка параметров диалога выбора файла
	OpenDialog1->Title = "Выберите файл";
	OpenDialog1->Filter = "Все файлы (*.*)|*.*";
	OpenDialog1->Options.Clear();
	OpenDialog1->Options << ofFileMustExist << ofHideReadOnly;

	// Отображение диалога и обработка результата
	if (OpenDialog1->Execute())
	{
		// Получение выбранного пути к файлу
		UnicodeString selectedFilePath = OpenDialog1->FileName;

		// Вывод пути к файлу (или выполните нужные действия с путем)
//		ShowMessage("Выбранный файл: " + selectedFilePath);
		RootFolderPathEdit1->Text = selectedFilePath;
		RootFolderPathEdit2->Text = selectedFilePath;
	}

	delete OpenDialog1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ProfileBtnClick(TObject *Sender)
{
	 SaveDataToConfig(userConfigSection, L"rootFolderPath", RootFolderPathEdit1->Text);
	 TabControl5->Visible = false;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::selectFolderBtnClick(TObject *Sender)
{
	TFileOpenDialog *folderDialog = new TFileOpenDialog(NULL);

	folderDialog->Options = folderDialog->Options << fdoPickFolders;

	if (folderDialog->Execute())
	{
		ShowMessage(L"Выбранный путь: " + folderDialog->FileName);
	}
	else
	{
		ShowMessage(L"Выбор отменен");
	}
	fileFolderPath = folderDialog->FileName;
	RootFolderPathEdit1->Text = folderDialog->FileName;
	RootFolderPathEdit2->Text = folderDialog->FileName;
	LoadDataFromConfig();

	delete folderDialog;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __fastcall TForm1::SaveDataToConfig(const UnicodeString &section, const UnicodeString &key, const UnicodeString &value)
{
  // Используйте уникальное имя конфигурационного файла для вашего приложения
  TIniFile *iniFile = new TIniFile(configFileNamePath);

  try
  {
	iniFile->WriteString(section, key, value);
  }
  __finally
  {
	delete iniFile;
  }
}

void __fastcall TForm1::LoadDataFromConfig()
{
  TIniFile *iniFile = new TIniFile(configFileNamePath);
  try
  {
	username = iniFile->ReadString(userConfigSection, "username", "");
	rootFolderPath = iniFile->ReadString(userConfigSection, "rootFolderPath", "");

	TDateTime currentDate = Date();
	System::Word year;
	System::Word month;
	System::Word day;
	DecodeDate(currentDate, year, month, day);
	//228
	if(!IsKeyExists(IntToStr(month) + L"." + IntToStr(year), L"day_" + IntToStr(day - 1))){
		SaveDataToConfig(IntToStr(month) + L"." + IntToStr(year), L"day_" + IntToStr(day - 1), L"0");
	}
	dayTime = StrToInt( iniFile->ReadString(IntToStr(month) + L"." + IntToStr(year), L"day_" + IntToStr(day - 1), "") );
	CurrentDateText->Caption = L"Current date:   " + IntToStr(day) + L"." + IntToStr(month) + L"." + IntToStr(year);
  }
  __finally
  {
	delete iniFile;
  }
}
//---------------------------------------------------------------------------
void TForm1::WriteArrayToConfig(const UnicodeString &section, const UnicodeString &key, std::vector<UnicodeString>& arr)
{
  TIniFile *iniFile = new TIniFile(configFileNamePath);

  try
  {
	iniFile->WriteString(section, section + "_size", arr.size());
	for(int i = 0; i < arr.size(); i++){
		iniFile->WriteString(section, key + "_" + IntToStr(i), arr[i]);
	}
  }
  __finally
  {
	delete iniFile;
  }
}

void TForm1::LoadArrayFromConfig(const UnicodeString &section, const UnicodeString &key, std::vector<UnicodeString>& arr)
{
  arr.clear();
  TIniFile *iniFile = new TIniFile(configFileNamePath);
  try
  {
	if (iniFile->SectionExists(section))
	{
		arraySize = StrToInt(iniFile->ReadString(section, section + "_size", ""));

		for(int i = 0; i < arraySize; i++){
			arr.push_back(iniFile->ReadString(section, key + "_" + IntToStr(i), ""));
		}
	}
  }
  __finally
  {
	delete iniFile;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __fastcall TForm1::SubjectsComboBoxChange(TObject *Sender)
{
	Timer1 -> Enabled = false;
	StartBtn -> Caption = "Start";
}

void __fastcall TForm1::TimeComboBoxChange(TObject *Sender)
{
	Timer1 -> Enabled = false;
	StartBtn -> Caption = "Start";

	TStringList *timeParts = new TStringList();
	timeParts->Delimiter = ':';
	timeParts->DelimitedText = TimeComboBox->Text;

	// Переменные для часов, минут и секунд
	H = StrToInt(timeParts->Strings[0]);
	M = StrToInt(timeParts->Strings[1]);
	S = StrToInt(timeParts->Strings[2]);

	TimeText -> Caption = TimeComboBox->Text;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::SubjectsListGetEditText(TObject *Sender, int ACol, int ARow,
		  UnicodeString &Value)
{
	ReloadSubjectsData(Now());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void __fastcall TForm1::LoadDataOfDay()
{
	TotalTimeText->Caption = formatTotalTime(StrToInt(dayTime));

	TimeComboBox->Items->Clear();
	TimeComboBox->Items->Add("03 : 00 : 00");
	TimeComboBox->Items->Add("02 : 30 : 00");
	TimeComboBox->Items->Add("02 : 00 : 00");
	TimeComboBox->Items->Add("01 : 30 : 00");
	TimeComboBox->Items->Add("01 : 00 : 00");
	TimeComboBox->Items->Add("00 : 30 : 00");
	TimeComboBox->Items->Add("00 : 15 : 00");

	TIniFile *iniFile = new TIniFile(configFileNamePath);
	SubjectsComboBox->Items->Clear();
	SubjectsComboBox->Items->Add("");

	try
	{
		if (iniFile->SectionExists(subjectsConfigSection))
		{

			LoadArrayFromConfig(subjectsConfigSection, "name", subjectsNames);
			LoadArrayFromConfig(subjectsConfigSection, "interval", subjectsIntervals);

			SubjectsList->RowCount = subjectsNames.size();
			for(int i = 0; i < subjectsNames.size(); i++){
				SubjectsList->Cells[0][i] = subjectsNames[i];
				SubjectsList->Cells[1][i] = subjectsIntervals[i];

				SubjectsComboBox->Items->Strings[i] = subjectsNames[i];
			}
		}
		else
		{
			ShowMessage("The section does not exist!");
		}
	}
	__finally
	{
		delete iniFile;
	}
}

void __fastcall TForm1::LoadSubjectsData(TDate date){
	System::Word year;
	System::Word month;
	System::Word day;
	DecodeDate(date, year, month, day);

	subjectsNames.clear();
	subjectsIntervals.clear();

	int daysInMonth = DaysInMonth(year, month);
	SubjectsChart->Title->Text->Text = getMonthName(month);

	LoadArrayFromConfig(subjectsConfigSection, "name",     subjectsNames);
	LoadArrayFromConfig(subjectsConfigSection, "interval", subjectsIntervals);

	Series2->Clear();
	for(int i = 0; i < subjectsNames.size(); i++){
		Series2->Add( StrToInt(subjectsIntervals[i]), subjectsNames[i], lookoutColors[i % subjectsNames.size()]);
	}
}

void __fastcall TForm1::ReloadSubjectsData(TDate date){
	System::Word year;
	System::Word month;
	System::Word day;

	DecodeDate(date, year, month, day);
	subjectsNames.clear();
	subjectsIntervals.clear();

    int daysInMonth = DaysInMonth(year, month);
	SubjectsChart->Title->Text->Text = getMonthName(month);

	arraySize = SubjectsList->RowCount;
	for(int i = 0; i < arraySize; i++){
		if((SubjectsList->Cells[0][i].Length() > 0) && (SubjectsList->Cells[1][i].Length() > 0)){
			subjectsNames.push_back(SubjectsList->Cells[0][i]);
			subjectsIntervals.push_back(SubjectsList->Cells[1][i]);
		}
	}

	WriteArrayToConfig(subjectsConfigSection, "name",     subjectsNames);
	WriteArrayToConfig(subjectsConfigSection, "interval", subjectsIntervals);

	Series2->Clear();
	for(int i = 0; i < subjectsNames.size(); i++){
		Series2->Add( StrToInt(subjectsIntervals[i]), subjectsNames[i], lookoutColors[i % subjectsNames.size()]);
	}

}

void __fastcall TForm1::ReloadDaysData(TDate date){
	System::Word year;
	System::Word month;
	System::Word day;
	DecodeDate(date, year, month, day);

	// Заносим данные
	int daysInMonth = DaysInMonth(year, month);
	SaveDataToConfig(IntToStr(month) + L"." + IntToStr(year), IntToStr(month) + L"." + IntToStr(year) + L"_size", IntToStr(daysInMonth));
//	if(!IsSectionExists(IntToStr(month) + L"." + IntToStr(year))){
//		SaveDataToConfig(IntToStr(month) + L"." + IntToStr(year), IntToStr(month) + L"." + IntToStr(year) + L"_size", IntToStr(daysInMonth));
//	}
	for(int i = 0; i < daysInMonth; i++){
		if(!IsKeyExists(IntToStr(month) + L"." + IntToStr(year), L"day_" + IntToStr(i))){
			SaveDataToConfig(IntToStr(month) + L"." + IntToStr(year), L"day_" + IntToStr(i), L"0");
		}
	}


	LoadArrayFromConfig(IntToStr(month) + L"." + IntToStr(year), "day", completedTasksTime);

	Series1->Clear();
	for(int i = 0; i < completedTasksTime.size(); i++){
		if(StrToInt(completedTasksTime[i]) == 0){
			Series1->Add(StrToInt(completedTasksTime[i]), i + 1, clWhite);
		}else{
			Series1->Add(StrToInt(completedTasksTime[i]), i + 1, lookoutColors[i % subjectsNames.size()]);
		}
	}

//	for(int i = completedTasksTime.size(); i < daysInMonth; i++){
//		Series1->Add(0.0 , i + 1, colors [ i % (sizeof(colors) / sizeof(colors[0])) ] );
//	}

	Chart1->Title->Text->Text = getMonthName(month);
}

void __fastcall TForm1::LoadDaysData(TDate date){
	System::Word year;
	System::Word month;
	System::Word day;
	DecodeDate(date, year, month, day);

	// Получаем количество дней в текущем месяце
	int daysInMonth = DaysInMonth(year, month);
	SaveDataToConfig(IntToStr(month) + L"." + IntToStr(year), IntToStr(month) + L"." + IntToStr(year) + L"_size", IntToStr(daysInMonth));
//	if(!IsSectionExists(IntToStr(month) + L"." + IntToStr(year))){
//		SaveDataToConfig(IntToStr(month) + L"." + IntToStr(year), IntToStr(month) + L"." + IntToStr(year) + L"_size", IntToStr(daysInMonth));
//	}

	for(int i = 0; i < daysInMonth; i++){
		if(!IsKeyExists(IntToStr(month) + L"." + IntToStr(year), L"day_" + IntToStr(i))){
			SaveDataToConfig(IntToStr(month) + L"." + IntToStr(year), L"day_" + IntToStr(i), L"0");
		}
	}


	LoadArrayFromConfig(IntToStr(month) + L"." + IntToStr(year), "day", completedTasksTime);

	Series1->Clear();
	for(int i = 0; i < completedTasksTime.size(); i++){
		if(StrToInt(completedTasksTime[i]) == 0){
			Series1->Add(StrToInt(completedTasksTime[i]), i + 1, clWhite);
		}else{
			Series1->Add(StrToInt(completedTasksTime[i]), i + 1, lookoutColors[i % subjectsNames.size()]);
		}
	}

	for(int i = completedTasksTime.size(); i < daysInMonth; i++){
		Series1->Add(0.0 , i + 1, clWhite);
	}
	Chart1->Title->Text->Text = getMonthName(month);
}

void __fastcall TForm1::LoadTasksData(TDate date){
	System::Word year;
	System::Word month;
	System::Word day;
	DecodeDate(date, year, month, day);

	int daysInMonth = DaysInMonth(year, month);
	int daysInConfig = 0;

	for(int i = 0; i < daysInMonth; i++){
		if(!IsSectionExists(IntToStr(i+1) + L"." + IntToStr(month) + L"." + IntToStr(year))){
			SaveDataToConfig(IntToStr(i+1) + L"." + IntToStr(month) + L"." + IntToStr(year), IntToStr(i+1) + L"." + IntToStr(month) + L"." + IntToStr(year) + L"_size", L"0");
		}
	}

	LoadArrayFromConfig(IntToStr(day) + L"." + IntToStr(month) + L"." + IntToStr(year), IntToStr(day) + L"." + IntToStr(month) + L"." + IntToStr(year) + L"_task" , completedSubjectsNames);
	LoadArrayFromConfig(IntToStr(day) + L"." + IntToStr(month) + L"." + IntToStr(year), IntToStr(day) + L"." + IntToStr(month) + L"." + IntToStr(year) + L"_time" , completedSubjectsTime);

	dayTime = 0;
	for (int i = 0; i < completedSubjectsNames.size(); i++){
		ToDoList->Cells[0][i] = completedSubjectsNames[i];
		ToDoList->Cells[1][i] = ConvertMinutesToTime(StrToInt(completedSubjectsTime[i]));
		dayTime += StrToInt(completedSubjectsTime[i]);
	}

	SaveDataToConfig(IntToStr(month) + L"." + IntToStr(year), L"day_" + IntToStr(day - 1), dayTime);
}


 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteSubjectBtnClick(TObject *Sender)
{
	if(SubjectsList->RowCount > 1){
		SubjectsList->RowCount = SubjectsList->RowCount -1;
		SubjectsList->Cells[0][SubjectsList->RowCount] = "";
		SubjectsList->Cells[1][SubjectsList->RowCount] = "";
		subjectsNames.pop_back();
		ReloadSubjectsData(Now());
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AddSubjectBtnClick(TObject *Sender)
{
	SubjectsList->RowCount = SubjectsList->RowCount + 1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ReturnBtnClick(TObject *Sender)
{
	TabControl6->Visible = false;
	TabControl4->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::EditSubjectBtnClick(TObject *Sender)
{
    TabControl4->Visible = false;
	TabControl6->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SubjectsListKeyPress(TObject *Sender, System::WideChar &Key)
{
	// Проверяем, что пользователь вводит только числа
	if (SubjectsList->Col == 1 && (!isdigit(Key) && Key != 8)) // 8 - код клавиши Backspace
	{
		Key = 0; // Отменяем ввод, если символ не является цифрой и не является Backspace
	}
}
//---------------------------------------------------------------------------










void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
//	subjectsNames.clear();
//	subjectsIntervals.clear();
//	arraySize = SubjectsList->RowCount;
//	for(int i = 0; i < arraySize; i++){
//		subjectsNames.push_back(SubjectsList->Cells[0][i]);
//		subjectsIntervals.push_back(SubjectsList->Cells[1][i]);
//	}
//
//	WriteArrayToConfig(subjectsConfigSection, "name",     subjectsNames);
//	WriteArrayToConfig(subjectsConfigSection, "interval", subjectsIntervals);
}
void __fastcall TForm1::FormResize(TObject *Sender)
{
    int newWidth = ClientWidth;
    int newHeight = ClientHeight;


	TabControl1->Left = 0;
	TabControl1->Top = 0;
    TabControl2->Left = 0;
	TabControl2->Top = 0;
	TabControl3->Left = 0;
	TabControl3->Top = 0;
	TabControl4->Left = 0;
	TabControl4->Top = 0;
	TabControl5->Left = 0;
	TabControl5->Top = 0;
	TabControl6->Left = 0;
	TabControl6->Top = 0;

	TabControl1->Width = newWidth;
	TabControl1->Height = newHeight;
	TabControl2->Width = newWidth;
	TabControl2->Height = newHeight;
	TabControl3->Width = newWidth;
	TabControl3->Height = newHeight;
	TabControl4->Width = newWidth;
	TabControl4->Height = newHeight;
	TabControl5->Width = newWidth;
	TabControl5->Height = newHeight;
	TabControl6->Width = newWidth;
	TabControl6->Height = newHeight;
	Chart1->Width = newWidth;
	Chart1->Height = newHeight;
	SubjectsChart->Width = newWidth;
	SubjectsChart->Height = newHeight;

	SubjectsBox->Left = (ClientWidth - SubjectsBox->Width) / 2;
	SubjectsBox->Top = (ClientHeight - SubjectsBox->Height) / 3;

	ProfileBox->Left = (ClientWidth - ProfileBox->Width) / 2;
	ProfileBox->Top = (ClientHeight - ProfileBox->Height) / 3;

	DataBox->Left = (ClientWidth - DataBox->Width) / 2;
	DataBox->Top = (ClientHeight - DataBox->Height) / 3;

	if(newWidth < 700){
		TimerGroupBox->Align = alTop;
		ToDoBox->Align = alBottom;


		TimerContainer->Margins->Left = (ClientWidth - TimerGroupBox->Width) / 2;
		TimerContainer->Left = (ClientWidth - TimerContainer->Width) / 2;
		TimerContainer->Top = 10;
		TimerContainer->Height = newHeight;
		TimerGroupBox->Height  = newHeight;
		ToDoBox->Visible = false;
	}else{
		ToDoBox->Align = alLeft;
        ToDoBox->Visible = true;
		TimerGroupBox->Align = alRight;
		TimerContainer->Height = 2 * newHeight/3;
		TimerGroupBox->Height = 2 * newHeight/3;

		ToDoList->Top = (ClientHeight - ProfileBox->Height) / 3;
		TimerContainer->Top = (ClientHeight - ProfileBox->Height) / 3;

		ToDoBox->Align = alLeft;
		TimerContainer->Left = (ClientWidth/2 - TimerContainer->Width) / 2 + 7;
		TimerContainer->Top = (ClientHeight - TimerContainer->Height) / 3;

		TimerGroupBox->Width = ClientWidth / 2;

		ToDoBox->Width = ClientWidth / 2;
		ToDoList->Top = (ClientHeight - TimerContainer->Height) / 3;

		ToDoList->DefaultColWidth = (ClientWidth / 6) - 1;
		ToDoList->Width = (ToDoList->DefaultColWidth + 1) * ToDoList->ColCount;
		ToDoList->Height = ToDoList->DefaultRowHeight * ToDoList->RowCount + 10;
		ToDoList->Left = (ClientWidth / 2 - ToDoList->DefaultColWidth * ToDoList->ColCount)/2;
		ToDoList->Margins->Top = 50;
    }
}
//---------------------------------------------------------------------------
