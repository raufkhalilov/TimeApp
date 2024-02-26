//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include "pies.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.FileCtrl.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.DBCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>

#include <vector>
#include <System.SysUtils.hpp>
#include <sstream>
#include <tchar.h>
#include <iostream>
#include <string>

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TTabControl *TabControl1;
	TStringGrid *ToDoList;
	TLabel *TimeText;
	TButton *StartBtn;
	TTimer *Timer1;
	TComboBox *SubjectsComboBox;
	TTabControl *TabControl2;
	TMainMenu *MainMenu1;
	TMenuItem *MenuDay;
	TMenuItem *MenuDays;
	TMenuItem *MenuProfile;
	TTabControl *TabControl3;
	TMenuItem *MenuSchedules;
	TTabControl *TabControl4;
	TGroupBox *GroupBox1;
	TLabel *Label4;
	TTabControl *TabControl5;
	TLabel *Label6;
	TButton *Button1;
	TBitBtn *BitBtn1;
	TEdit *RootFolderPathEdit2;
	TLabel *Label8;
	TFileOpenDialog *FileOpenDialog1;
	TBitBtn *EditSubjectBtn;
	TLabel *Label9;
	TEdit *RootFolderPathEdit1;
	TBitBtn *BitBtn2;
	TButton *Button2;
	TChart *Chart1;
	TChart *SubjectsChart;
	TTabControl *TabControl6;
	TStringGrid *SubjectsList;
	TBitBtn *DeleteSubjectBtn;
	TBitBtn *AddSubjectBtn;
	TBitBtn *ReturnBtn;
	TLabel *Label3;
	TLabel *Label5;
	TPieSeries *Series2;
	TBarSeries *Series1;
	TComboBox *TimeComboBox;
	TLabel *Label10;
	TGroupBox *GroupBox2;
	TLabel *TotalTimeText;
	TGroupBox *GroupBox3;
	TLabel *CurrentDateText;
	TGroupBox *ProfileBox;
	TGroupBox *GroupBox4;
	TGroupBox *TimerGroupBox;
	TGroupBox *GroupBox5;
	TPanel *TimerContainer;
	TGroupBox *ToDoBox;
	TGroupBox *SubjectsBox;
	TGroupBox *DataBox;
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall StartBtnClick(TObject *Sender);
	void __fastcall MenuDayClick(TObject *Sender);
	void __fastcall MenuDaysClick(TObject *Sender);
	void __fastcall MenuProfileClick(TObject *Sender);
	void __fastcall selectFolderBtnClick(TObject *Sender);
	void __fastcall RootFolderBtnClick(TObject *Sender);
	void __fastcall ProfileBtnClick(TObject *Sender);
	void __fastcall MenuSchedulesClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall SubjectsComboBoxChange(TObject *Sender);
	void __fastcall SubjectsListGetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value);
	void __fastcall DeleteSubjectBtnClick(TObject *Sender);
	void __fastcall AddSubjectBtnClick(TObject *Sender);
	void __fastcall ReturnBtnClick(TObject *Sender);
	void __fastcall EditSubjectBtnClick(TObject *Sender);
	void __fastcall SubjectsListKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall TimeComboBoxChange(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);




private:	// User declarations

public:		// User declarations
	int H = 1, M = 30, S = 0;
	int subjectsAmount = 0;
	int arraySize = 0;
	int k = 0;
	int completedTasksCounter = 0;
	int dayTime = 0;
	UnicodeString username = L"";
    UnicodeString rootFolderPath = L"";
	UnicodeString fileFolderPath = L"";
	UnicodeString configFileNamePath = L"C:/Users/Рауф/Downloads/Telegram Desktop/icon150/TimeAppConfig.ini";
	UnicodeString subjectsConfigSection = L"SubjectsData";
	UnicodeString userConfigSection = L"UserData";
	UnicodeString daysConfigSection = L"DaysData";
	UnicodeString excelFilePath = "days.xlsx";
	UnicodeString timerCurrentSubject;

	//Day, Subjects
	std::vector<UnicodeString> subjectsNames;
	std::vector<UnicodeString> subjectsIntervals;
	//_ToDoList
	std::vector<UnicodeString> completedSubjectsNames;
	std::vector<UnicodeString> completedSubjectsTime;
	//Days
	std::vector<UnicodeString> completedTasksTime;

//    TColor lookoutColors[12] = {
//		(TColor)RGB(0xD6, 0x99, 0x77),   // $00D69977
//		(TColor)RGB(0x68, 0xCF, 0xFF),   // $0068CFFF
//		(TColor)RGB(0x47, 0x7F, 0xFF),   // $00477FFF
//		(TColor)RGB(0xDB, 0xCA, 0x81),   // $00DBCA81
//		(TColor)RGB(0x9E, 0x73, 0x5E),   // $009E735E
//		(TColor)RGB(0x96, 0xAE, 0x50),   // $0096AE50
//		(TColor)RGB(0x41, 0x3B, 0xE6),   // $00413BE6
//		(TColor)RGB(0x90, 0xF3, 0xFF),   // $0090F3FF
//		(TColor)RGB(0xD1, 0xEA, 0x90),   // $00D1EA90
//		(TColor)RGB(0xA3, 0xA3, 0xA3),   // $00A3A3A3
//		(TColor)RGB(0xC0, 0xFF, 0xFF),   // $00C0FFFF
//		(TColor)RGB(0xE7, 0xE7, 0xE7)    // $00E7E7E7
//	};


	const TColor lookoutColors[14] = {
		(TColor)RGB(0x66, 0x99, 0xFF),   // $006699FF
		(TColor)RGB(0x66, 0x66, 0xFF),   // $006666FF
		(TColor)RGB(0xFF, 0xCC, 0x99),   // $00FFCC99
		(TColor)RGB(0x66, 0x99, 0x66),   // $00669966
		(TColor)RGB(0x99, 0xCC, 0xCC),   // $0099CCCC
		(TColor)RGB(0xCC, 0x66, 0x99),   // $00CC6699
		(TColor)RGB(0x66, 0x66, 0xCC),   // $006666CC
		(TColor)RGB(0x99, 0xCC, 0xFF),   // $0099CCFF
		(TColor)RGB(0xFF, 0x66, 0x99),   // $00FF6699
		(TColor)RGB(0xCC, 0xCC, 0xCC),   // $00CCCCCC
		(TColor)RGB(0xCC, 0xFF, 0x66),   // $00CCFF66
		(TColor)RGB(0xFF, 0x99, 0x66),   // $00FF9966
		(TColor)RGB(0x99, 0x66, 0x99),   // $00996699
		(TColor)RGB(0xFF, 0xCC, 0xCC)    // $00FFCCCC
	};

	__fastcall TForm1(TComponent* Owner);
    void WriteArrayToConfig(const UnicodeString &section, const UnicodeString &key, std::vector<UnicodeString>& arr);
	void LoadArrayFromConfig(const UnicodeString &section, const UnicodeString &key, std::vector<UnicodeString>& arr);
	void __fastcall LoadDataOfDay();
	void __fastcall LoadSubjectsData(TDate date);
	void __fastcall ReloadSubjectsData(TDate date);
	void __fastcall SaveDataToConfig(const UnicodeString &section, const UnicodeString &key, const UnicodeString &value);
	void __fastcall LoadDataFromConfig();
	void __fastcall ReloadDaysData(TDate date);
	void __fastcall LoadTasksData(TDate date);
    void __fastcall LoadDaysData(TDate date);


	UnicodeString formatTime(int hours, int minutes, int seconds) {
		UnicodeString formattedTime = "";

		formattedTime += (hours < 10 ? L"0" : L"")   + IntToStr(hours) + L" : ";
		formattedTime += (minutes < 10 ? L"0" : L"") + IntToStr(minutes) + L" : ";
		formattedTime += (seconds < 10 ? L"0" : L"") + IntToStr(seconds);

		return formattedTime;
	}

	UnicodeString formatTotalTime(int time) {
		UnicodeString formattedTime = IntToStr(time);
		UnicodeString formattedPreTime = "";
		for(int i = 0; i < 4 - formattedTime.Length(); i++){
			formattedPreTime += L"0";
		}

		return formattedPreTime + formattedTime;
	}

	int timeStringToMinutes(const UnicodeString& timeString)
	{
		// Разбиваем строку на компоненты
		TStringList* components = new TStringList;
		components->Delimiter = ':';
		components->DelimitedText = timeString;

		UnicodeString hoursStr = components->Strings[0];
		UnicodeString minutesStr = components->Strings[1];

		// Преобразуем компоненты в числа
		int hours = StrToInt(hoursStr);
		int minutes = StrToInt(minutesStr);

		// Вычисляем общее количество минут
		int totalMinutes = hours * 60 + minutes;

		// Освобождаем память
		delete components;

		return totalMinutes;
	}

	UnicodeString getMonthName(int monthNumber) {
		const UnicodeString months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

		if (monthNumber >= 1 && monthNumber <= 12) {
			return months[monthNumber - 1];
		} else {
			return "Invalid month number";
		}
	}


	UnicodeString ConvertMinutesToTime(int totalMinutes) {
		UnicodeString time = "";
		int hours = totalMinutes / 60;
		int minutes = totalMinutes % 60;
		int seconds = 0;

		time += (hours < 10 ? L"0" : L"")   + IntToStr(hours) + L" : ";
		time += (minutes < 10 ? L"0" : L"") + IntToStr(minutes) + L" : ";
		time += (seconds < 10 ? L"0" : L"") + IntToStr(seconds);

		return time;
	}

	bool IsSectionExists(const UnicodeString& sectionName)
	{
		TIniFile *iniFile = new TIniFile(configFileNamePath);

		try
		{
			// Проверяем существование секции
			return iniFile->SectionExists(sectionName);
		}
		__finally
		{
			delete iniFile;
		}
	}

	bool IsKeyExists(const UnicodeString& sectionName, const UnicodeString& keyName)
	{
		TIniFile *iniFile = new TIniFile(configFileNamePath);

		try
		{
			// Проверяем существование ключа в указанной секции
			return iniFile->ValueExists(sectionName, keyName);
		}
		__finally
		{
			delete iniFile;
		}
	}

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
