#include <cppconsui/Application.h>
#include <cppconsui/Window.h>
#include <cppconsui/Label.h>
#include <cppconsui/Button.h>
#include <cppconsui/TreeView.h>
#include <cppconsui/Keys.h>

// TreeViewWindow class
class TreeViewWindow
: public Window
{
	public:
		/* This is a main window, make sure it can not be closed with ESC key
		 * by overriding Close() method. */
		static TreeViewWindow *Instance();
		virtual void Close() {}

		virtual void ScreenResized();

	protected:

	private:
		TreeViewWindow();
		virtual ~TreeViewWindow() {}
		TreeViewWindow(const TreeViewWindow&);
		TreeViewWindow& operator=(const TreeViewWindow&);
};

TreeViewWindow *TreeViewWindow::Instance()
{
	static TreeViewWindow instance;
	return &instance;
}

TreeViewWindow::TreeViewWindow()
: Window(0, 0, 0, 0)
{
	TreeView *tree;
	TreeView::NodeReference node;
	TreeView::NodeReference node2;

	AddWidget(*(new Label(20, 1, "Press F10 to quit.")), 1, 1);

	tree = new TreeView(30, 12);
	AddWidget(*tree, 1, 3);
	SetInputChild(*tree);

	node = tree->AddNode(tree->Root(), *(new Button("Button node A")));
	node2 = tree->AddNode(node, *(new Button("Button node A-1")));
	tree->AddNode(node2, *(new Button("Button node A-1-a")));
	tree->AddNode(node2, *(new Button("Button node A-1-b")));
	tree->AddNode(node2, *(new Button("Button node A-1-c")));
	tree->AddNode(node, *(new Button("Button node A-2")));
	tree->AddNode(node, *(new Button("Button node A-3")));

	node = tree->AddNode(tree->Root(), *(new Label("Label node B")));
	tree->AddNode(node, *(new Label("Label node B-1")));
	tree->AddNode(node, *(new Label("Label node B-2")));
	tree->AddNode(node, *(new Label("Label node B-3")));

	node = tree->AddNode(tree->Root(), *(new Button("Button node C")));
	tree->AddNode(node, *(new Button("Button node C-1")));
	tree->AddNode(node, *(new Button("Button node C-2")));
	tree->AddNode(node, *(new Button("Button node C-3")));
}

void TreeViewWindow::ScreenResized()
{
	MoveResize(0, 0, WINDOWMANAGER->getScreenW(),
			WINDOWMANAGER->getScreenH());
}

// TestApp class

#define CONTEXT_TESTAPP "testapp"

class TestApp
: public Application
{
	public:
		static TestApp *Instance();

		virtual void Run();
		virtual void Quit();

		virtual void ScreenResized() {}

		// ignore every message
		static void g_log_func_(const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data)
			{}

	protected:

	private:
		TestApp();
		TestApp(const TestApp&);
		TestApp& operator=(const TestApp&);
		virtual ~TestApp() {}

		DECLARE_SIG_REGISTERKEYS();
		static bool RegisterKeys();
		void DeclareBindables();
};

TestApp *TestApp::Instance()
{
	static TestApp instance;
	return &instance;
}

TestApp::TestApp()
: Application()
{
	g_log_set_default_handler(g_log_func_, this);

	DeclareBindables();
}

void TestApp::Run()
{
	windowmanager->Add(TreeViewWindow::Instance());

	Application::Run();
}

void TestApp::DeclareBindables()
{
	DeclareBindable(CONTEXT_TESTAPP, "quit",
			sigc::mem_fun(this, &TestApp::Quit),
			InputProcessor::Bindable_Override);
}

DEFINE_SIG_REGISTERKEYS(TestApp, RegisterKeys);
bool TestApp::RegisterKeys()
{
	RegisterKeyDef(CONTEXT_TESTAPP, "quit",
			"Quit TestApp.", Keys::FunctionTermKey(10));
	return true;
}

void TestApp::Quit()
{
	Application::Quit();
}

// main function
int main()
{
	setlocale(LC_ALL, "");

	TestApp *app = TestApp::Instance();

	app->Run();

	return 0;
}
