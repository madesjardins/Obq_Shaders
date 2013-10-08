// This file is part of the Lens Distortion Plugin Kit
// Software is provided "as is" - no warranties implied.
// (C) 2011,2012,2013 - Science-D-Visions. Current version: 1.4


//
//
//	3DEqualizer4  -  Lens Distortion Plugin Interface
//	
//		File	tde4_ld_plugin.h
//		Author	RS
//		Date	01.10.10
//	
//		Version	1.0.5
	

#pragma once


// parameter types...
enum tde4_ldp_ptype { TDE4_LDP_STRING, TDE4_LDP_DOUBLE, TDE4_LDP_INT, TDE4_LDP_FILE, TDE4_LDP_TOGGLE, TDE4_LDP_ADJUSTABLE_DOUBLE };

//! @brief Lens Distortion Plugin Base Class
class tde4_ld_plugin
{
public:

	virtual ~tde4_ld_plugin() {}
	

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
	virtual bool getParameterDefaultValue(const char *identifier, char *v) { v[0] = 0; return false; }
	virtual bool getParameterDefaultValue(const char *identifier, int &v) { v = 0; return false; }
	virtual bool getParameterDefaultValue(const char *identifier, bool &v) { v = false; return false; }
	
	//! returns range for adjustable double parameters...
	virtual bool getParameterRange(const char *identifier, double &a, double &b) = 0;

	//! @brief set parameter values...
	//! parameters predefined by 3DE4: "tde4_focal_length_cm", "tde4_filmback_width_cm", "tde4_filmback_height_cm",
	//! "tde4_lens_center_offset_x_cm", "tde4_lens_center_offset_y_cm", "tde4_pixel_aspect", "tde4_custom_focus_distance_cm"
	virtual bool setParameterValue(const char *identifier, double v) = 0;
	virtual bool setParameterValue(const char *identifier, char *v) { return false; }
	virtual bool setParameterValue(const char *identifier, int v) { return false; }
	virtual bool setParameterValue(const char *identifier, bool v) { return false; }
	
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
	
};


// extern "C" style pseudo constructor & destructor...
typedef tde4_ld_plugin* tde4ldp_create_fct_t();
typedef void tde4ldp_destroy_fct_t(tde4_ld_plugin*);



