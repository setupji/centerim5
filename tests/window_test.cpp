#include <cppconsui/Application.h>
#include <cppconsui/Window.h>
#include <cppconsui/Label.h>
#include <cppconsui/Keys.h>
#include <vector>

/** TestWindow class. */
class TestWindow
: public Window
{
	public:
		TestWindow(int number, int x, int y, int w, int h);

		virtual void ScreenResized();

	protected:
		Label *label;

	private:
		TestWindow(const TestWindow &);
		TestWindow &operator=(const TestWindow &);
		virtual ~TestWindow() {}
};

TestWindow::TestWindow(int number, int x, int y, int w, int h)
: Window(x, y, w, h, new Border())
{
	gchar *t = g_strdup_printf("Win %d", number);
	label = new Label(*this, 2, 1, w - 4, 1, t);
	g_free(t);
	AddWidget(label);
}

void TestWindow::ScreenResized()
{
	Redraw();
}

/** TestApp class. */

#define CONTEXT_TESTAPP "testapp"

class TestApp
: public Application
{
	public:
		static TestApp &Instance();

		virtual void Run();
		virtual void Quit();

		virtual void ScreenResized() {}

		// ignore every message
		static void g_log_func_(const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data)
			{}

	protected:
		std::vector<Window *> wins;

	private:
		TestApp();
		TestApp(const TestApp &);
		TestApp &operator=(const TestApp &);
		virtual ~TestApp();

		DECLARE_SIG_REGISTERKEYS();
		static bool RegisterKeys();
		void DeclareBindables();
};

TestApp &TestApp::Instance()
{
	static TestApp instance;
	return instance;
}

TestApp::TestApp()
: Application()
{
	g_log_set_default_handler(g_log_func_, this);

	DeclareBindables();
}

TestApp::~TestApp()
{
	for (std::vector<Window *>::iterator i = wins.begin(); i != wins.end(); i++)
		delete *i;
}

void TestApp::Run()
{
	for (int i = 1; i <= 4; i++) {
		Window *w = new TestWindow(i, (i - 1) % 2 * 30, (i - 1) / 2 * 10, 30, 10);
		wins.push_back(w);
		windowmanager->Add(w);
	}

	Application::Run();
}

void TestApp::DeclareBindables()
{
	DeclareBindable(CONTEXT_TESTAPP, "quit", sigc::mem_fun(this, &TestApp::Quit),
			InputProcessor::Bindable_Override);
}

DEFINE_SIG_REGISTERKEYS(TestApp, RegisterKeys);
bool TestApp::RegisterKeys()
{
	RegisterKeyDef(CONTEXT_TESTAPP, "quit", _("Quit TestApp."), Keys::Instance()->Key_f10());
	return true;
}

void TestApp::Quit()
{
	Application::Quit();
}

/** Main function. */
int main(void)
{
	setlocale(LC_ALL, "");

	TestApp *app = &TestApp::Instance();

	app->Run();

	return 0;
}
