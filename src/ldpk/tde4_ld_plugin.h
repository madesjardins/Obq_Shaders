// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013 - Science-D-Visions. Current version: 1.7


//
//
//	3DEqualizer4  -  Lens Distortion Plugin Interface
//	
//		File	tde4_ld_plugin.h
//		Author	RS
//		Date	01.10.10
//	
//		Version: see macro

#pragma once
#include <cmath>
#include <iostream>
#include <ai.h>

#define TDE4_LDP_VERSION "1.0.8"

// parameter types...
enum tde4_ldp_ptype { TDE4_LDP_STRING=0, TDE4_LDP_DOUBLE=1, TDE4_LDP_INT=2, TDE4_LDP_FILE=3, TDE4_LDP_TOGGLE=4, TDE4_LDP_ADJUSTABLE_DOUBLE=5 };

//! @brief Lens Distortion Plugin Base Class
class tde4_ld_plugin
{
public:

	virtual ~tde4_ld_plugin() {}
	
	const char* getVersionString() const
		{ return TDE4_LDP_VERSION; }

	//! returns a name for the model as to show up in the GUI (maximum length of "name": 100 bytes)...
	virtual bool getModelName(char *model) = 0;

	//! returns the number of plugin parameters...
	virtual bool getNumParameters(int &n) = 0;

	//! returns "identifier" name of parameter "i" (maximum length of "identifier": 100 bytes)...
	virtual bool getParameterName(int i, char *identifier) = 0;
	
	//! @brief returns type of given parameter...
	//! The method should return false, if the parameter addressed by identifier does not exist.
	virtual bool getParameterType(const char *identifier, tde4_ldp_ptype &type) = 0;
	
	//! returns default value for given parameter (maximum length of "char *v": 1000 bytes)......
	virtual bool getParameterDefaultValue(const char *identifier, double &v) = 0;
	virtual bool getParameterDefaultValue(const char *identifier, char *v)
		{
		//std::cerr << "getParameterDefaultValue(" << identifier << ",char*): unimplemented method." << std::endl;
		AiMsgError("getParameterDefaultValue(%s,char*): unimplemented method.", identifier);
		v[0] = 0;
		return false;
		}
	virtual bool getParameterDefaultValue(const char *identifier, int &v)
		{
		//std::cerr << "getParameterDefaultValue(" << identifier << ",int&): unimplemented method." << std::endl;
		AiMsgError("getParameterDefaultValue(%s,int&): unimplemented method.", identifier);
		v = 0;
		return false;
		}
	virtual bool getParameterDefaultValue(const char *identifier, bool &v)
		{
		//std::cerr << "getParameterDefaultValue(" << identifier << ",bool&): unimplemented method." << std::endl;
		AiMsgError("getParameterDefaultValue(%s,bool&): unimplemented method.", identifier);
		v = false;
		return false;
		}
	
	//! returns range for adjustable double parameters...
	virtual bool getParameterRange(const char *identifier, double &a, double &b) = 0;

	//! @brief set parameter values...
	//! parameters predefined by 3DE4: "tde4_focal_length_cm", "tde4_filmback_width_cm", "tde4_filmback_height_cm",
	//! "tde4_lens_center_offset_x_cm", "tde4_lens_center_offset_y_cm", "tde4_pixel_aspect", "tde4_custom_focus_distance_cm"
	virtual bool setParameterValue(const char *identifier, double v) = 0;
	virtual bool setParameterValue(const char *identifier, const char *v)
		{
		//std::cerr << "setParameterValue(" << identifier << ",const char* " << v << "): unimplemented method." << std::endl;
		AiMsgError("setParameterValue(%s,const char* \"%s\"): unimplemented method.", identifier,v);
		return false;
		}
	virtual bool setParameterValue(const char *identifier, int v)
		{
		//std::cerr << "setParameterValue(" << identifier << ",int " << v << "): unimplemented method." << std::endl;
		AiMsgError("setParameterValue(%s,const char* \"%i\"): unimplemented method.", identifier,v);
		return false;
		}
	virtual bool setParameterValue(const char *identifier, bool v)
		{
		AiMsgError("setParameterValue(%s,const char* \"%s\"): unimplemented method.", identifier,(v?"True":"False"));
		//std::cerr << "setParameterValue(" << identifier << ",bool " << v << "): unimplemented method." << std::endl;
		return false;
		}
	
	//! prepare the current set of parameters...
	virtual bool initializeParameters() = 0;

	//! warp/unwarp 2D points...
	virtual bool undistort(double x0, double y0, double &x1, double &y1) = 0;
	virtual bool distort(double x0, double y0, double &x1, double &y1) = 0;
	
	//! potentially more efficient function which uses initial values...
	virtual bool distort(double x0, double y0, double x1_start , double y1_start, double &x1, double &y1) { return distort(x0,y0,x1,y1); }
	
	//! parameter derivatives to be used for distortion grid controls' matrix calculation routine...
	virtual bool providesParameterDerivatives() { return false; }
	//! dx_dy is an array with 2 * num_parameters elements (x- and y- component for each of n derivatives)
	virtual bool calcParameterDerivatives(double x, double y, int num_parameters, double *dx_dy) { return false; }
	
	//! @brief calculate the Jacobian matrix of the undistort()-Method. Overwrite this,
	//! if you know the Jacobian for your own lens distortion model.
	virtual bool getJacobianMatrix(double x0, double y0, double &m00, double &m01, double &m10, double &m11)
		{
		bool ok = true;
		double eps = 1e-4,x00,x10,x01,x11,y00,y10,y01,y11;
		ok &= undistort(x0 - eps,y0,x00,y00);
		ok &= undistort(x0 + eps,y0,x01,y01);
		ok &= undistort(x0,y0 - eps,x10,y10);
		ok &= undistort(x0,y0 + eps,x11,y11);
		m00 = (x01 - x00) / (2.0 * eps);
		m01 = (x11 - x10) / (2.0 * eps);
		m10 = (y01 - y00) / (2.0 * eps);
		m11 = (y11 - y10) / (2.0 * eps);
		return ok;
		}
	//! @brief calculate the mixed derivatives ("twist vector") of the undistort()-Method. Overwrite this,
	//! if you know the twist vector for your own lens distortion model.
	virtual bool getTwistVector(double x0, double y0, double &t0, double &t1)
		{
		bool ok = true;
		double eps = 1e-4,x00,x10,x01,x11,y00,y10,y01,y11;
		ok &= undistort(x0 - eps,y0 - eps,x00,y00);
		ok &= undistort(x0 + eps,y0 - eps,x10,y10);
		ok &= undistort(x0 - eps,y0 + eps,x01,y01);
		ok &= undistort(x0 + eps,y0 + eps,x11,y11);
		t0 = (x11 - x01 - x10 + x00) / (4.0 * eps * eps);
		t1 = (y11 - y01 - y10 + y00) / (4.0 * eps * eps);
		return ok;
		}
	//! Iterate around the specified box, undistort the points and compute the bounding box
	virtual void getBoundingBoxUndistort(double xa_in,double ya_in,double xb_in,double yb_in,double& xa_out,double& ya_out,double& xb_out,double& yb_out,int nx = 32,int ny = 32)
		{
		xa_out = HUGE_VAL;
		ya_out = HUGE_VAL;
		xb_out = -HUGE_VAL;
		yb_out = -HUGE_VAL;
		double dx_in = xb_in - xa_in,dy_in = yb_in - ya_in;
// Avoid double evaluation at the corners.
		for(int ix = 0;ix < nx;++ix)
			{
			double xa = xa_in + dx_in * double(ix    ) / nx;
			double xb = xa_in + dx_in * double(ix + 1) / nx;
			accumulate_undistort(xa,ya_in,xa_out,ya_out,xb_out,yb_out);
			accumulate_undistort(xb,yb_in,xa_out,ya_out,xb_out,yb_out);
			}
		for(int iy = 0;iy < ny;++iy)
			{
			double ya = ya_in + dy_in * double(iy + 1) / ny;
			double yb = ya_in + dy_in * double(iy    ) / ny;
			accumulate_undistort(xa_in,ya,xa_out,ya_out,xb_out,yb_out);
			accumulate_undistort(xb_in,yb,xa_out,ya_out,xb_out,yb_out);
			}
		}
	//! Iterate around the specified box, distort the points and compute the bounding box
	virtual void getBoundingBoxDistort(double xa_in,double ya_in,double xb_in,double yb_in,double& xa_out,double& ya_out,double& xb_out,double& yb_out,int nx = 32,int ny = 32)
		{
		xa_out = HUGE_VAL;
		ya_out = HUGE_VAL;
		xb_out = -HUGE_VAL;
		yb_out = -HUGE_VAL;
		double dx_in = xb_in - xa_in,dy_in = yb_in - ya_in;
// Avoid double evaluation at the corners.
		for(int ix = 0;ix < nx;++ix)
			{
			double xa = xa_in + dx_in * double(ix    ) / nx;
			double xb = xa_in + dx_in * double(ix + 1) / nx;
			accumulate_distort(xa,ya_in,xa_out,ya_out,xb_out,yb_out);
			accumulate_distort(xb,yb_in,xa_out,ya_out,xb_out,yb_out);
			}
		for(int iy = 0;iy < ny;++iy)
			{
			double ya = ya_in + dy_in * double(iy + 1) / ny;
			double yb = ya_in + dy_in * double(iy    ) / ny;
			accumulate_distort(xa_in,ya,xa_out,ya_out,xb_out,yb_out);
			accumulate_distort(xb_in,yb,xa_out,ya_out,xb_out,yb_out);
			}
		}
private:
// Methods we need in the bounding box methods.
	void accumulate_undistort(double x0,double y0,double& xa_out,double& ya_out,double& xb_out,double& yb_out)
		{
		double x1,y1;
		undistort(x0,y0,x1,y1);
		xa_out = std::min(xa_out,x1);
		ya_out = std::min(ya_out,y1);
		xb_out = std::max(xb_out,x1);
		yb_out = std::max(yb_out,y1);
		}
	void accumulate_distort(double x0,double y0,double& xa_out,double& ya_out,double& xb_out,double& yb_out)
		{
		double x1,y1;
		distort(x0,y0,x1,y1);
		xa_out = std::min(xa_out,x1);
		ya_out = std::min(ya_out,y1);
		xb_out = std::max(xb_out,x1);
		yb_out = std::max(yb_out,y1);
		}
};


// extern "C" style pseudo constructor & destructor...
typedef tde4_ld_plugin* tde4ldp_create_fct_t();
typedef void tde4ldp_destroy_fct_t(tde4_ld_plugin*);

