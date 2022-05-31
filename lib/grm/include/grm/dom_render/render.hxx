#ifndef RENDER_HXX
#define RENDER_HXX

#include <iostream>
#include <optional>

#include <grm/dom_render/context.hxx>
#include <grm/dom_render/graphics_tree/Element.hxx>
#include <grm/dom_render/graphics_tree/Document.hxx>
#include "gr.h"


namespace GR
{

class Render : public Document
{
  /*!
   * The GR::Render class is used for creating or modifying elements that can also be processed by this class
   * to create plots etc.
   *
   * GR::Render has an std::shared_ptr GR::Context as a private member for storing certain datatypes
   */

public:
  static std::shared_ptr<Render> createRender();
  std::shared_ptr<Element> createPolymarker(const std::string &x_key, std::optional<std::vector<double>> x,
                                            const std::string &y_key, std::optional<std::vector<double>> y,
                                            const std::shared_ptr<Context> &extContext = nullptr, int marker_type = 0,
                                            double marker_size = 0.0, int marker_colorind = 0);

  std::shared_ptr<Element> createPolymarker(double x, double y, int marker_type = 0, double marker_size = 0.0,
                                            int marker_colorind = 0);

  std::shared_ptr<Element> createPolyline(const std::string &x_key, std::optional<std::vector<double>> x,
                                          const std::string &y_key, std::optional<std::vector<double>> y,
                                          const std::shared_ptr<Context> &extContext = nullptr, int line_type = 0,
                                          double line_width = 0.0, int line_colorind = 0);

  std::shared_ptr<Element> createPolyline(double x1, double x2, double y1, double y2, int line_type = 0,
                                          double line_width = 0.0, int line_colorind = 0);

  std::shared_ptr<Element> createText(double x, double y, const std::string &text);

  std::shared_ptr<Element> createFillArea(const std::string &x_key, std::optional<std::vector<double>> x,
                                          const std::string &y_key, std::optional<std::vector<double>> y,
                                          const std::shared_ptr<Context> &extContext = nullptr, int fillintstyle = 0,
                                          int fillstyle = 0, int fillcolorind = -1);

  std::shared_ptr<Element> createCellArray(double xmin, double xmax, double ymin, double ymax, int dimx, int dimy,
                                           int scol, int srow, int ncol, int nrow, const std::string &color_key,
                                           std::optional<std::vector<int>> color,
                                           const std::shared_ptr<Context> &extContext = nullptr);

  std::shared_ptr<Element> createAxes(double x_tick, double y_tick, double x_org, double y_org, int major_x,
                                      int major_y, double tick_size);

  std::shared_ptr<Element> createGrid(double x_tick, double y_tick, double x_org, double y_org, int major_x,
                                      int major_y);

  std::shared_ptr<Element> createGroup();

  std::shared_ptr<Element> createGroup(const std::string &name);

  std::shared_ptr<Element> createDrawImage(double xmin, double ymin, double xmax, double ymax, int width, int height,
                                           const std::string &data_key, std::optional<std::vector<int>> data, int model,
                                           const std::shared_ptr<Context> &extContext = nullptr);

  std::shared_ptr<Element> createDrawArc(double xmin, double xmax, double ymin, double ymax, double a1, double a2);

  std::shared_ptr<Element> createFillArc(double xmin, double xmax, double ymin, double ymax, double a1, double a2,
                                         int fillintstyle = 0, int fillstyle = 0, int fillcolorind = -1);

  std::shared_ptr<Element> createDrawRect(double xmin, double xmax, double ymin, double ymax);

  std::shared_ptr<Element> createFillRect(double xmin, double xmax, double ymin, double ymax, int fillintstyle = 0,
                                          int fillstyle = 0, int fillcolorind = -1);

  std::shared_ptr<Element> createQuiver(const std::string &x_key, std::optional<std::vector<double>> x,
                                        const std::string &y_key, std::optional<std::vector<double>> y,
                                        const std::string &u_key, std::optional<std::vector<double>> u,
                                        const std::string &v_key, std::optional<std::vector<double>> v, int color,
                                        const std::shared_ptr<Context> &extContext = nullptr);

  std::shared_ptr<Element> createContour(const std::string &px_key, std::optional<std::vector<double>> px,
                                         const std::string &py_key, std::optional<std::vector<double>> py,
                                         const std::string &h_key, std::optional<std::vector<double>> h,
                                         const std::string &pz_key, std::optional<std::vector<double>> pz, int major_h,
                                         const std::shared_ptr<Context> &extContext = nullptr);


  std::shared_ptr<Element> createContourf(const std::string &px_key, std::optional<std::vector<double>> px,
                                          const std::string &py_key, std::optional<std::vector<double>> py,
                                          const std::string &h_key, std::optional<std::vector<double>> h,
                                          const std::string &pz_key, std::optional<std::vector<double>> pz, int major_h,
                                          const std::shared_ptr<Context> &extContext = nullptr);

  std::shared_ptr<Element> createHexbin(int x_length, const std::string &x_key, std::optional<std::vector<double>> x,
                                        const std::string &y_key, std::optional<std::vector<double>> y, int nbins,
                                        const std::shared_ptr<Context> &extContext = nullptr);

  std::shared_ptr<Element> createNonUniformCellArray(const std::string &x_key, std::optional<std::vector<double>> x,
                                                     const std::string &y_key, std::optional<std::vector<double>> y,
                                                     int dimx, int dimy, int scol, int srow, int ncol, int nrow,
                                                     const std::string &color_key,
                                                     std::optional<std::vector<int>> color,
                                                     const std::shared_ptr<Context> &extContext = nullptr);
  //        std::shared_ptr<Element>
  //        createGridIt(int nd, std::string& xd_key, std::optional<std::vector<double>> xd, std::string& yd_key,
  //        std::optional<std::vector<double>> yd, std::string& zd_key, std::optional<std::vector<double>> zd, int nx,
  //        int ny, )

  std::shared_ptr<Element> createSurface(const std::string &px_key, std::optional<std::vector<double>> px,
                                         const std::string py_key, std::optional<std::vector<double>> py,
                                         const std::string &pz_key, std::optional<std::vector<double>> pz, int option,
                                         const std::shared_ptr<Context> &extContext = nullptr);

  std::shared_ptr<Element> createGrid3d(double x_tick, double y_tick, double z_tick, double x_org, double y_org,
                                        double z_org, int major_x, int major_y, int major_z);

  std::shared_ptr<Element> createAxes3d(double x_tick, double y_tick, double z_tick, double x_org, double y_org,
                                        double z_org, int major_x, int major_y, int major_z, double tick_size);

  std::shared_ptr<Element> createPolyline3d(const std::string &x_key, std::optional<std::vector<double>> x,
                                            const std::string &y_key, std::optional<std::vector<double>> y,
                                            const std::string &z_key, std::optional<std::vector<double>> z,
                                            const std::shared_ptr<Context> &extContext = nullptr);

  std::shared_ptr<Element> createPolymarker3d(const std::string &x_key, std::optional<std::vector<double>> x,
                                              const std::string &y_key, std::optional<std::vector<double>> y,
                                              const std::string &z_key, std::optional<std::vector<double>> z,
                                              const std::shared_ptr<Context> &extContext = nullptr);

  std::shared_ptr<Element>
  createGR3DrawMesh(int mesh, int n, const std::string &positions_key, std::optional<std::vector<double>> positions,
                    const std::string &directions_key, std::optional<std::vector<double>> directions,
                    const std::string &ups_key, std::optional<std::vector<double>> ups, const std::string &colors_key,
                    std::optional<std::vector<double>> colors, const std::string &scales_key,
                    std::optional<std::vector<double>> scales, const std::shared_ptr<Context> &extContext = nullptr);

  std::shared_ptr<Element> createVolume(int nx, int ny, int nz, const std::string &data_key,
                                        std::optional<std::vector<double>> data, int algorithm, double dmin,
                                        double dmax, const std::shared_ptr<Context> &extContext = nullptr);

  std::shared_ptr<Element> createVolume(int nx, int ny, int nz, const std::string &data_key,
                                        std::optional<std::vector<double>> data, int algorithm,
                                        const std::string &dmin_key, double dmin, const std::string &dmax_key,
                                        double dmax, const std::shared_ptr<Context> &extContext = nullptr);

  std::shared_ptr<Element> createClearWS();

  std::shared_ptr<Element> createUpdateWS();

  std::shared_ptr<Element> createDrawGraphics(const std::string &data_key, std::optional<std::vector<int>> data,
                                              const std::shared_ptr<Context> &extContext = nullptr);

  std::shared_ptr<Element> createTriSurface(const std::string &px_key, std::optional<std::vector<double>> px,
                                            const std::string &py_key, std::optional<std::vector<double>> py,
                                            const std::string &pz_key, std::optional<std::vector<double>> pz,
                                            const std::shared_ptr<Context> &extContext = nullptr);

  std::shared_ptr<Element> createTriContour(const std::string &px_key, std::optional<std::vector<double>> px,
                                            const std::string &py_key, std::optional<std::vector<double>> py,
                                            const std::string &pz_key, std::optional<std::vector<double>> pz,
                                            const std::string &levels_key, std::optional<std::vector<double>> levels,
                                            const std::shared_ptr<Context> &extContext = nullptr);

  std::shared_ptr<Element> createGR3Clear();

  std::shared_ptr<Element> createGR3DeleteMesh(int mesh);

  //  std::shared_ptr<Element> createGR3DeleteMesh(std::shared_ptr<Element> &element);

  std::shared_ptr<Element> createGR3DrawImage(double xmin, double xmax, double ymin, double ymax, int width, int height,
                                              int drawable_type);

  //  std::shared_ptr<Element> createGR3IsoSurfaceMesh(const std::string& data_key, std::optional<std::vector<int>>
  //  *data, int isolevel, unsigned int dim_x, unsigned int dim_y, unsigned int dim_z, unsigned int stride_x, unsigned
  //  int stride_y, unsigned int stride_z, double step_x, double step_y, double step_z, double offset_x, double
  //  offset_y, double offset_z);

  std::shared_ptr<Element> createShadePoints(const std::string &x_key, std::optional<std::vector<double>> x,
                                             const std::string &y_key, std::optional<std::vector<double>> y, int xform,
                                             int w, int h, const std::shared_ptr<Context> &extContext = nullptr);

  //! Modifierfunctions
  void setViewport(const std::shared_ptr<Element> &element, double xmin, double xmax, double ymin, double ymax);

  void setWindow(const std::shared_ptr<Element> &element, double xmin, double xmax, double ymin, double ymax);

  void setMarkerType(const std::shared_ptr<Element> &element, int type);

  void setMarkerType(const std::shared_ptr<Element> &element, const std::string &types_key,
                     const std::vector<int> &types, const std::shared_ptr<Context> &extContext = nullptr);

  void setMarkerSize(const std::shared_ptr<Element> &element, const std::string &sizes_key,
                     const std::vector<double> &sizes, const std::shared_ptr<Context> &extContext = nullptr);

  void setMarkerSize(const std::shared_ptr<Element> &element, double size);

  void setMarkerColorInd(const std::shared_ptr<Element> &element, const std::string &colorinds_key,
                         const std::vector<int> &colorinds, const std::shared_ptr<Context> &extContext = nullptr);

  void setMarkerColorInd(const std::shared_ptr<Element> &element, int color);

  void setLineType(const std::shared_ptr<Element> &element, const std::string &types_key, const std::vector<int> &types,
                   const std::shared_ptr<Context> &extContext = nullptr);

  void setLineType(const std::shared_ptr<Element> &element, int type);

  void setLineWidth(const std::shared_ptr<Element> &element, const std::string &widths_key,
                    const std::vector<double> &widths, const std::shared_ptr<Context> &extContext = nullptr);

  void setLineWidth(const std::shared_ptr<Element> &element, double width);

  void setLineColorInd(const std::shared_ptr<Element> &element, const std::string &colorinds_key,
                       const std::vector<int> &colorinds, const std::shared_ptr<Context> &extContext = nullptr);

  void setLineColorInd(const std::shared_ptr<Element> &element, int color);

  void setTextFontPrec(const std::shared_ptr<Element> &element, int font, int prec);

  void setCharUp(const std::shared_ptr<Element> &element, double ux, double uy);

  void setTextAlign(const std::shared_ptr<Element> &element, int horizontal, int vertical);

  void setColorRep(const std::shared_ptr<Element> &element, int index, double red, double green, double blue);


  void setLineSpec(const std::shared_ptr<Element> &element, const std::string &spec);

  void setFillIntStyle(const std::shared_ptr<Element> &element, int index);

  void setFillColorInd(const std::shared_ptr<Element> &element, int color);

  void setFillStyle(const std::shared_ptr<Element> &element, int index);

  void setScale(const std::shared_ptr<Element> &element, int scale);

  void setWindow3d(const std::shared_ptr<Element> &element, double xmin, double xmax, double ymin, double ymax,
                   double zmin, double zmax);

  void setSpace3d(const std::shared_ptr<Element> &element, double phi, double theta, double fov,
                  double camera_distance);

  void setSelntran(const std::shared_ptr<Element> &element, int transform);

  void setGR3BackgroundColor(const std::shared_ptr<Element> &element, double red, double green, double blue,
                             double alpha);

  void setGR3CameraLookAt(const std::shared_ptr<Element> &element, double camera_x, double camera_y, double camera_z,
                          double center_x, double center_y, double center_z, double up_x, double up_y, double up_z);

  void setTextColorInd(const std::shared_ptr<Element> &element, int index);

  void setBorderColorInd(const std::shared_ptr<Element> &element, int index);

  void selectClipXForm(const std::shared_ptr<Element> &element, int form);

  void setCharHeight(const std::shared_ptr<Element> &element, double height);

  void setTransparency(const std::shared_ptr<Element> &element, double alpha);

  void setResampleMethod(const std::shared_ptr<Element> &element, int resample);

  void setTextEncoding(const std::shared_ptr<Element> &element, int encoding);

  void render();                                           // render doc and render context
  void render(const std::shared_ptr<Context> &extContext); // render doc and external context
  void render(const std::shared_ptr<Document> &document);  // external doc and render context
  static void render(const std::shared_ptr<Document> &document,
                     const std::shared_ptr<Context> &extContext); // external doc and external context; could be static


private:
  Render();
  std::shared_ptr<Context> context;
};
} // namespace GR

#endif
