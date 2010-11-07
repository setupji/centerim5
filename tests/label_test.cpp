#include <cppconsui/CoreManager.h>
#include <cppconsui/Window.h>
#include <cppconsui/Label.h>
#include <cppconsui/Keys.h>

// LabelWindow class
class LabelWindow
: public Window
{
  public:
    /* This is a main window, make sure it can not be closed with ESC key by
     * overriding Close() method. */
    static LabelWindow *Instance();
    virtual void Close() {}

    virtual void ScreenResized();

  protected:

  private:
    LabelWindow();
    virtual ~LabelWindow() {}
    LabelWindow(const LabelWindow&);
    LabelWindow& operator=(const LabelWindow&);
};

LabelWindow *LabelWindow::Instance()
{
  static LabelWindow instance;
  return &instance;
}

LabelWindow::LabelWindow()
: Window(0, 0, 0, 0)
{
  Label *label;

  label = new Label(
    20, // width
    1, // height
    "Press F10 to quit."); // text
  /* Add label to container, container takes widget ownership and deletes it
   * when necessary.
   */
  AddWidget(*label, 1, 1);

  label = new Label(20, 1, "Too wide string, too wide string, too wide string");
  AddWidget(*label, 1, 3);

  label = new Label(20, 3, "Multiline label, multiline label, multiline label");
  AddWidget(*label, 1, 5);

  // unicode test
  label = new Label(30, 3,
      "\x56\xc5\x99\x65\xc5\xa1\x74\xc3\xad\x63\xc3\xad\x20\x70\xc5\x99"
      "\xc3\xad\xc5\xa1\x65\x72\x79\x20\x73\x65\x20\x64\x6f\xc5\xbe\x61"
      "\x64\x6f\x76\x61\x6c\x79\x20\xc3\xba\x70\x6c\x6e\xc4\x9b\x20\xc4"
      "\x8d\x65\x72\x73\x74\x76\xc3\xbd\x63\x68\x20\xc5\x99\xc3\xad\x7a"
      "\x65\xc4\x8d\x6b\xc5\xaf\x2e\x0a");
  AddWidget(*label, 1, 9);

  label = new Label("Autosize");
  AddWidget(*label, 1, 13);

  const gchar *long_text = "Lorem ipsum dolor sit amet, consectetur"
    "adipiscing elit. Duis dui dui, interdum eget tempor auctor, viverra"
    "suscipit velit. Phasellus vel magna odio. Duis rutrum tortor at nisi"
    "auctor tincidunt. Mauris libero neque, faucibus sit amet semper in, "
    "dictum ut tortor. Duis lacinia justo non lorem blandit ultrices."
    "Nullam vel purus erat, eget aliquam massa. Aenean eget mi a nunc"
    "lacinia consectetur sed a neque. Cras varius, dolor nec rhoncus"
    "ultricies, leo ipsum adipiscing mi, vel feugiat ipsum urna id "
    "metus. Cras non pulvinar nisi. Vivamus nisi lorem, tempor tristique"
    "cursus sit amet, ultricies interdum metus. Nullam tortor tortor, "
    "iaculis sed tempor non, tincidunt ac mi. Quisque id diam vitae diam"
    "dictum facilisis eget ac lacus. Vivamus at gravida felis. Curabitur"
    "fermentum mattis eros, ut auctor urna tincidunt vitae. Praesent"
    "tincidunt laoreet lobortis.";

  label = new Label(-1, 10, long_text);
  AddWidget(*label, 42, 13);

  label = new Label(40, -1, long_text);
  AddWidget(*label, 1, 24);

  label = new Label(-1, -1, long_text);
  AddWidget(*label, 42, 24);
}

void LabelWindow::ScreenResized()
{
  MoveResize(0, 0, COREMANAGER->GetScreenWidth(),
      COREMANAGER->GetScreenHeight());
}

// TestApp class

#define CONTEXT_TESTAPP "testapp"

class TestApp
: public InputProcessor
{
  public:
    static TestApp *Instance();

    void Run();

    // ignore every message
    static void g_log_func_(const gchar *log_domain, GLogLevelFlags log_level,
        const gchar *message, gpointer user_data)
      {}

  protected:

  private:
    CoreManager *mngr;

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
: InputProcessor()
{
  mngr = CoreManager::Instance();

  g_log_set_default_handler(g_log_func_, this);

  DeclareBindables();
}

void TestApp::Run()
{
  mngr->AddWindow(*LabelWindow::Instance());
  mngr->SetTopInputProcessor(*this);
  mngr->EnableResizing();
  mngr->StartMainLoop();
}

void TestApp::DeclareBindables()
{
  DeclareBindable(CONTEXT_TESTAPP, "quit", sigc::mem_fun(mngr,
        &CoreManager::QuitMainLoop), InputProcessor::BINDABLE_OVERRIDE);
}

DEFINE_SIG_REGISTERKEYS(TestApp, RegisterKeys);
bool TestApp::RegisterKeys()
{
  RegisterKeyDef(CONTEXT_TESTAPP, "quit", "Quit TestApp.",
      Keys::FunctionTermKey(10));
  return true;
}

// main function
int main()
{
  setlocale(LC_ALL, "");

  TestApp *app = TestApp::Instance();

  app->Run();

  return 0;
}
