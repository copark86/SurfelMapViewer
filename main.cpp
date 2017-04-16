#include <iostream>
#include <pangolin/pangolin.h>

#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include "Parse.h"



typedef struct {
    double r,g,b;
} COLOUR;

COLOUR GetColour(double v,double vmin,double vmax)
{
    COLOUR c = {1.0,1.0,1.0}; // white
    double dv;

    if (v < vmin)
        v = vmin;
    if (v > vmax)
        v = vmax;
    dv = vmax - vmin;

    if (v < (vmin + 0.25 * dv)) {
        c.r = 0;
        c.g = 4 * (v - vmin) / dv;
    } else if (v < (vmin + 0.5 * dv)) {
        c.r = 0;
        c.b = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
    } else if (v < (vmin + 0.75 * dv)) {
        c.r = 4 * (v - vmin - 0.5 * dv) / dv;
        c.b = 0;
    } else {
        c.g = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
        c.b = 0;
    }

    return(c);
}


using std::vector;

struct Surfel {
    float r,g,b;
    float nx,ny,nz;
    float x,y,z,t,c;
};


struct CustomType
{
  CustomType()
    : x(0), y(0.0f) {}

  CustomType(int x, float y, std::string z)
    : x(x), y(y), z(z) {}

  int x;
  float y;
  std::string z;
};

std::ostream& operator<< (std::ostream& os, const CustomType& o){
  os << o.x << " " << o.y << " " << o.z;
  return os;
}

std::istream& operator>> (std::istream& is, CustomType& o){
  is >> o.x;
  is >> o.y;
  is >> o.z;
  return is;
}







void SampleMethod()
{
    std::cout << "You typed ctrl-r or pushed reset" << std::endl;
}



void Load(std::string *filename,vector<Surfel> * Surfel_load) {


    FILE *fp;
    fp = fopen(filename->c_str(), "r");


    float r, g, b;
    float nx, ny, nz;
    float x, y, z, t, c;


    while (fscanf(fp, "%f %f %f %f %f %f %f %f %f %f %f", &x, &y, &z, &t, &c, &nx, &ny, &nz, &r, &g, &b) != EOF) {

        Surfel tmp;
        tmp.x = x;
        tmp.y = y;
        tmp.z = z;
        tmp.t = t;
        tmp.c = c;
        tmp.nx = nx;
        tmp.ny = ny;
        tmp.nz = nz;
        tmp.r = r;
        tmp.g = g;
        tmp.b = b;

        Surfel_load->push_back(tmp);
    }


    fclose(fp);


}







void ListDrawSurfels(GLuint * disp_list,vector<Surfel> * Surfel_load,int draw_type,float rad,float min_confi) {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    *disp_list=glGenLists(1);
    glNewList(*disp_list,GL_COMPILE);


    glEnable(GL_DEPTH_TEST);


    GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 0.7, 0.0, 0.2, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);


//glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_LIGHTING);



//glLoadIdentity();
    pangolin::glDrawAxis(100);

    Surfel o = Surfel_load->at(Surfel_load->size()-1);

    float maxt = o.t;
    //while(fscanf(fp, "%f %f %f %f %f",&x,&y,&z,&t,&c)!=EOF)
    for(int k = 0;k <Surfel_load->size();k++)
    {

        Surfel o = Surfel_load->at(k);

        if(o.c>min_confi){
            glPushMatrix();
            if(draw_type == 0)
            {
                glColor3f(o.r,o.g,o.b);
            }else if(draw_type == 1)
            {
                COLOUR c = GetColour(o.t,0,maxt);
                glColor3f(c.r,c.g,c.b);

            }else if(draw_type == 2)
            {
                COLOUR c = GetColour(o.c,0,50.0);
                glColor3f(c.r,c.g,c.b);
            }
            else
                glColor3f(o.r,o.g,o.b);


            glTranslatef(o.x/1000,o.y/1000,o.z/1000);

            float rx,ry,rz;
            rz = 0;
            ry = atan2(o.nx,o.nz)/3.141592*180;
            rx = 0;
            rx = atan2(o.ny,sqrt(o.nx*o.nx+o.nz*o.nz))/3.141592*180;
            glRotatef(rz,0.0f,0.0f,1.0f);
            glRotatef(ry,0.0f,1.0f,0.0f);
            glRotatef(-rx,1.0f,0.0f,0.0f);

            /*
            glRotatef(o.nz,0.0f,0.0f,1.0f);
            glRotatef(o.ny,0.0f,1.0f,0.0f);
            glRotatef(o.nx,1.0f,0.0f,0.0f);
*/
            pangolin::glDrawCircle( 0, 0, rad);
            //pangolin::glDrawAxis(1);
            glPopMatrix();
        }
   

    }

    glEndList();


}



int main(int argc, char* argv[])
{



    std::string filename;

    Parse::get().arg(argc, argv, "-l", filename);






  // Load configuration data
 pangolin::ParseVarsFile("app.cfg");

  // Create OpenGL window in single line
  pangolin::CreateWindowAndBind("Surfel Viewer",640,480);
  
  // 3D Mouse handler requires depth testing to be enabled
  glEnable(GL_DEPTH_TEST);

  // Define Camera Render Object (for view / scene browsing)
  pangolin::OpenGlRenderState s_cam(
    pangolin::ProjectionMatrix(640,480,420,420,320,240,0.1,1000),
    pangolin::ModelViewLookAt(-0,0.5,3, 0,0,0, pangolin::AxisY)
  );

  const int UI_WIDTH = 180;

  // Add named OpenGL viewport to window and provide 3D Handler
  pangolin::View& d_cam = pangolin::CreateDisplay()
    .SetBounds(0.0, 1.0, pangolin::Attach::Pix(UI_WIDTH), 1.0, -640.0f/480.0f)
    .SetHandler(new pangolin::Handler3D(s_cam));

  // Add named Panel and bind to variables beginning 'ui'
  // A Panel is just a View with a default layout and input handling
  pangolin::CreatePanel("ui")
      .SetBounds(0.0, 1.0, 0.0, pangolin::Attach::Pix(UI_WIDTH));

  // Safe and efficient binding of named variables.
  // Specialisations mean no conversions take place for exact types
  // and conversions between scalar types are cheap.
  pangolin::Var<bool> filereload("ui.Load",false,false);
   pangolin::Var<bool> updateclick("ui.UpdateSetting",false,false);

  pangolin::Var<double> confidence_val("ui.confi",1,0,50);
  pangolin::Var<double> rad_val("ui.rad",0.3,0,1);




  pangolin::Var<bool> time_checkbox("ui.Time",false,true);
    pangolin::Var<bool> confidence_checkbox("ui.Confidence",false,true);
    pangolin::Var<bool> normal_checkbox("ui.Normal",false,true);




  pangolin::Var<int> num_surfels("ui.Surfels",2);
  //pangolin::Var<CustomType> any_type("ui.Some_Type", CustomType(0,1.2f,"Hello") );

  pangolin::Var<bool> save_window("ui.Save_Window",false,false);
  pangolin::Var<bool> save_view("ui.Save_View",false,false);

  pangolin::Var<bool> record_cube("ui.Record_Cube",false,false);

  // std::function objects can be used for Var's too. These work great with C++11 closures.
  pangolin::Var<std::function<void(void)> > reset("ui.Reset", SampleMethod);

  // Demonstration of how we can register a keyboard hook to alter a Var
  pangolin::RegisterKeyPressCallback(pangolin::PANGO_CTRL + 'b', pangolin::SetVarFunctor<double>("ui.A Double", 3.5));

  // Demonstration of how we can register a keyboard hook to trigger a method
  pangolin::RegisterKeyPressCallback(pangolin::PANGO_CTRL + 'r', SampleMethod);




    // surfel map related
    vector<Surfel> Surfel_load;
    GLuint disp_list;
    // load data from file
    Load(&filename,&Surfel_load);
    ListDrawSurfels(&disp_list,&Surfel_load,0,rad_val,confidence_val);





  // Default hooks for exiting (Esc) and fullscreen (tab).
  while( !pangolin::ShouldQuit() )
  {




    // Clear entire screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    


      // Reload pointcloud file
    if( pangolin::Pushed(filereload) ){
        Load(&filename,&Surfel_load);
        ListDrawSurfels(&disp_list,&Surfel_load,0,rad_val,confidence_val);
    }


      // Update settings and redraw
    if( pangolin::Pushed(updateclick) )
    {

        int draw_type=0;
        if( time_checkbox )
            draw_type=1; // color by time
        if( confidence_checkbox )
            draw_type=2; // color by confidence
        if( normal_checkbox )
            draw_type = 0; // otherwise

        // reset the draw list
        ListDrawSurfels(&disp_list,&Surfel_load,draw_type,rad_val,confidence_val);
    }
      num_surfels = Surfel_load.size();





    // button handlers
    if( pangolin::Pushed(save_window) )// save whole window
        pangolin::SaveWindowOnRender("window");

    if( pangolin::Pushed(save_view) )// save the view only
        d_cam.SaveOnRender("view");
    
    if( pangolin::Pushed(record_cube) )// record a video
        pangolin::DisplayBase().RecordOnRender("ffmpeg:[fps=50,bps=8388608,unique_filename]//screencap.avi");





    d_cam.Activate(s_cam);


      // call the list, this will redraw the serfels when view changed
    glCallList(disp_list);
/*
      pangolin::glDrawAxis(1);
      glTranslatef(1,1,1);

      glRotatef(0,0.0f,0.0f,1.0f);
      glRotatef(45.0f,0.0f,1.0f,0.0f);
      glRotatef(-35.24f,1.0f,0.0f,0.0f);

      pangolin::glDrawCircle( 0, 0, 0.5);
      pangolin::glDrawAxis(1);
*/

    pangolin::FinishFrame();
  }

  return 0;
}
















