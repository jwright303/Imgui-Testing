#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "imgui.h"
#include "implot.h"
#include <cmath>
#define GL_SILENCE_DEPRECATION


#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <GLFW/glfw3.h>
#include "boost/numeric/ublas/operation.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "joescan_pinchot.h"

static void glfw_error_callback(int error, const char* description)
{
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void update_rotation_matrix(int *rot_mat)
{
  return;
}

void rotate_points(int *img, int *rot_mat)
{
  return;
}

void update_matrix(boost::numeric::ublas::matrix<double> &RM, double x_rad, double y_rad) {
  // Update the rotation matrix
  RM(0,0) = cos(y_rad);  RM(0,1) = sin(x_rad) * sin(y_rad); RM(0,2) = cos(x_rad) * sin(y_rad);
  RM(1,0) = 0;           RM(1,1) = cos(x_rad);              RM(1,2) = -sin(x_rad);
  RM(2,0) = -sin(y_rad); RM(2,1) = sin(x_rad) * cos(y_rad); RM(2,2) = cos(x_rad) * cos(y_rad);
}

void plot_rot_data(double *x_data, double *y_data, int color_idx, 
              boost::numeric::ublas::matrix<double> &M, char *legend) {
  
  // Update the array to display the results
  for (int i = 0; i < 150; i++) {
    x_data[i] = M(i, 0);
    y_data[i] = M(i, 1);
  }
  
  ImPlot::SetNextMarkerStyle(ImPlotMarker_Square,
                              1,
                              ImPlot::GetColormapColor(color_idx),
                              IMPLOT_AUTO,
                              ImPlot::GetColormapColor(color_idx));

  ImPlot::PlotScatter(legend, x_data, y_data, 150);
}


int main(int argc, char ** argv) {
    int x = 0;
    int y = 0;
    printf("Hello World\n");
    GLFWwindow* window = nullptr;
    double x_data[1][150];
    double y_data[1][150];
    double theta = 0.0;
    bool mouse_state_down = false;
    ImVec2 prev {};
    
    double rad_step = M_PI/60.0;
    double step = M_PI/60.0;
    double x_rad = 0.0;
    double y_rad = 0.0;

    // typedef boost::multi_array<double, 2> array_type;
    // typedef array_type::index index;
    boost::numeric::ublas::matrix<double> A (JS_PROFILE_DATA_LEN, 3);
    boost::numeric::ublas::matrix<double> C (JS_PROFILE_DATA_LEN, 3);
    boost::numeric::ublas::matrix<double> D (JS_PROFILE_DATA_LEN, 3);
    boost::numeric::ublas::matrix<double> E (JS_PROFILE_DATA_LEN, 3);
    boost::numeric::ublas::matrix<double> B (JS_PROFILE_DATA_LEN, 3);
    boost::numeric::ublas::matrix<double> RM (3, 3);

    for (int i = 0; i < JS_PROFILE_DATA_LEN; i++) {
      for (int j = 0; j < 3; j++) {
        A(i, j) = 0;
        B(i, j) = 0;
        C(i, j) = 0;
        D(i, j) = 0;
        E(i, j) = 0;
        RM(i, j) = 0;
      }
    }

    x_data[0][0] = 0.0;
    y_data[0][0] = 0.0;

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        fprintf(stdout, "GLFW Error");
      return 1;
    }

    auto monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    window = glfwCreateWindow(1920,
                              1200,
                              "JoeScan JS50 ScanGui Example",
                              NULL,
                              NULL);
    if (nullptr == window) {
      return 1;
    }

    //glfwMaximizeWindow(window);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

     // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    int change_factor = 1.0;

    // Rotate the circle another step
    for (int i = 0; i < 150; i++) {
      B(i, 0) = 50.0 * cos(theta + (double(i) * step));
      B(i, 1) = 50.0 * sin(theta + (double(i) * step));
    }
    // theta += step;

    while (!glfwWindowShouldClose(window)) {
        if (!glfwGetWindowAttrib(window, GLFW_VISIBLE)) {
            continue;
        }

        // Rotate the circle another step
        for (int i = 0; i < 150; i++) {
          B(i, 0) = 50.0 * cos(theta + (double(i) * step));
          B(i, 1) = 50.0 * sin(theta + (double(i) * step));
        }
        theta += step/2;


        // Check if they are shift dragging the screen
        if (ImGui::IsKeyDown(ImGuiKey_ModShift)) {
          if (prev.x == 0 && prev.y == 0) {
            prev = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
          } else {
            ImVec2 cur = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
            if (cur.x != 0 && cur.y != 0) {
              if (std::abs(prev.x - cur.x) + std::abs(prev.y - cur.y) > 0) {
                // std::cout << cur.x - prev.x << " " << cur.y - prev.y << std::endl;
                y_rad += (cur.x - prev.x) * rad_step;
                x_rad += (cur.y - prev.y) * rad_step;
              }
            }
            prev = cur;
          }
        }
        y_rad += M_PI/360;
        x_rad += M_PI/300;

        update_matrix(RM, x_rad, y_rad);
        boost::numeric::ublas::axpy_prod(B, boost::numeric::ublas::trans(RM), A, true);  // A = B * RM

        update_matrix(RM, x_rad + (5*M_PI/360), y_rad + (5*M_PI/300));
        boost::numeric::ublas::axpy_prod(B, boost::numeric::ublas::trans(RM), C, true);  // A = B * RM

        update_matrix(RM, x_rad + (10*M_PI/360), y_rad + (10*M_PI/300));
        boost::numeric::ublas::axpy_prod(B, boost::numeric::ublas::trans(RM), D, true);  // A = B * RM
        
        update_matrix(RM, x_rad + (5*M_PI/360), y_rad + (5*M_PI/300));
        boost::numeric::ublas::axpy_prod(B, boost::numeric::ublas::trans(RM), E, true);  // A = B * RM

        // Update the array to display the results
        for (int i = 0; i < 150; i++) {
          x_data[0][i] = A(i, 0);
          y_data[0][i] = A(i, 1);
        }

        glfwPollEvents();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static float f = 0.0f;
        static int counter = 0;
#ifdef IMGUI_HAS_VIEWPORTV
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
#else
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
#endif
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        bool open = true;
        ImGui::Begin("ScanData",
                    &open,
                    ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);


        auto is_plot_sucess = ImPlot::BeginPlot("Profile Plot",
                                              "X [inches]",
                                              "Y [inches]",
                                              ImVec2(-1, -1),
                                              ImPlotFlags_Equal);
        if (!is_plot_sucess) {
            continue;
        }
        

        ImPlot::SetupAxesLimits(-50.0, 50.0, -50.0, 50.0);
        ImPlot::SetupFinish();

        char *lenged = new char[32];
        strcpy(lenged, "Data ");
        lenged[6]=char(0);
        for (int i = 0; i < 15; i ++) {
          update_matrix(RM, x_rad + ((i * 5) * M_PI/360), y_rad + ((i * 5) * M_PI/300));
          boost::numeric::ublas::axpy_prod(B, boost::numeric::ublas::trans(RM), A, true);  // A = B * RM
          
          for (int i = 0; i < 150; i++) {
            x_data[0][i] = A(i, 0);
            y_data[0][i] = A(i, 1);
          }
          lenged[5] = char(65 + i);
          plot_rot_data(x_data[0], y_data[0], i, A, lenged);
        }


        ImPlot::EndPlot();
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w,
                    clear_color.y * clear_color.w,
                    clear_color.z * clear_color.w,
                    clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
        
    }

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}