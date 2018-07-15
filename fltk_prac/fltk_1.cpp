#include<FL/Fl.H>
#include<FL/Fl_Window.H>
#include<FL/Fl_Box.H>
#include<FL/Fl_Button.H>

int main(int argc,char** argv){
    Fl_Window *window = new Fl_Window(1000,800);
    Fl_Box *box = new Fl_Box(20,40,300,100,"Hello,NNERO");
    box->box(FL_UP_BOX);
    box->labelfont(FL_BOLD+FL_ITALIC);
    box->labelsize(36);
    box->labeltype(FL_SHADOW_LABEL);
    Fl_Button *button = new Fl_Button(340,40,300,100,"Hello,World");
    button->type(FL_NORMAL_BUTTON);
    Fl_Button *radio = new Fl_Button(660,40,100,100,"haha");
    radio->type(FL_RADIO_BUTTON);
    window->align(FL_ALIGN_CENTER);
    window->end();
    window->show(argc,argv);
    return Fl::run();
}
