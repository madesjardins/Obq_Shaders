// A big thanks to Bruce Lindbloom for the info (www.brucelindbloom.com) and Wikipedia

#pragma once

#include <string>

#ifdef _WIN32
	#include <glm\glm.hpp>
#else
	#include <glm/glm.hpp>
#endif

// NOTE :
//	Matrices in GLM are column major, these are the indices
// 0 3 6
// 1 4 7
// 2 5 8

// Illuminant IDs
enum OIlluminantID {ILMNT_A, ILMNT_B, ILMNT_C, ILMNT_D50, ILMNT_D55, ILMNT_D65, ILMNT_D75, ILMNT_E, ILMNT_F2, ILMNT_F7, ILMNT_F11, ILMNT_D60};

// Gamma IDs
enum OGammaID {G_LINEAR, G_1p8, G_2p2, G_sRGB, G_2p4, G_REC709, G_LSTAR, G_REC2020};

// ColorSpace name
enum OCSID {CS_LINEAR_RGB, CS_ADOBE_RGB_1998, CS_APPLE_RGB, CS_BEST_RGB, CS_BETA_RGB, CS_BRUCE_RGB, CS_CIE_RGB, CS_COLORMATCH_RGB, CS_DON_RGB_4, CS_ECI_RGB_V2, CS_EKTA_SPACE_PS5, CS_NTSC_RGB, CS_PAL_SECAM_RGB, CS_PROPHOTO_RGB, CS_SMPTE_C_RGB, CS_sRGB, CS_WIDE_GAMUT_RGB, CS_REC709, CS_REC2020, CS_CIE_XYZ, CS_ALEXA_WIDE_GAMMUT_RGB, CS_ACES_CG};

// Column major Bradford Matrix
const glm::mat3 c_BradfordMatrix(0.8951f, -0.7502f, 0.0389f, 0.2664f, 1.7135f, -0.0685f, -0.1614f, 0.0367f,  1.0296f);
const glm::mat3 c_BradfordMatrixI(0.9869929f, 0.4323053f,  -0.0085287f, -0.1470543f,  0.5183603f,  0.0400428f,  0.1599627f, 0.0492912f, 0.9684867f);


// Illuminant ID to XYZ white reference values
void illuminantID2XYZ(OIlluminantID ilmnt, float &X, float &Y, float &Z)
{
	switch(ilmnt)
	{
	case ILMNT_A:
		X = 1.09850f;
		Y = 1.00000f;
		Z = 0.35585f;
		break;
	case ILMNT_B:
		X = 0.99072f;
		Y = 1.00000f;
		Z = 0.85223f;
		break;
	case ILMNT_C:
		X = 0.98074f;
		Y = 1.00000f;
		Z = 1.18232f;
		break;
	case ILMNT_D50:
		X = 0.96422f;
		Y = 1.00000f;
		Z = 0.82521f;
		break;
	case ILMNT_D55:
		X = 0.95682f;
		Y = 1.00000f;
		Z = 0.92149f;
		break;
	case ILMNT_D60:
		X = 0.95264f;
		Y = 1.00000f;
		Z = 1.00882f;
		break;
	case ILMNT_D65:
		X = 0.95047f;
		Y = 1.00000f;
		Z = 1.08883f;
		break;
	case ILMNT_D75:
		X = 0.94972f;
		Y = 1.00000f;
		Z = 1.22638f;
		break;
	case ILMNT_E:
		X = 1.00000f;
		Y = 1.00000f;
		Z = 1.00000f;
		break;
	case ILMNT_F2:
		X = 0.99186f;
		Y = 1.00000f;
		Z = 0.67393f;
		break;
	case ILMNT_F7:
		X = 0.95041f;
		Y = 1.00000f;
		Z = 1.08747f;
		break;
	case ILMNT_F11:
		X = 1.00962f;
		Y = 1.00000f;
		Z = 0.64350f;
		break;
	default:
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
		break;
	}
}

// Illuminant ID to XYZ white reference values
const char* illuminantID2text(OIlluminantID ilmnt)
{
	switch(ilmnt)
	{
	case ILMNT_A:
		return "A";
	case ILMNT_B:
		return "B";
	case ILMNT_C:
		return "C";
	case ILMNT_D50:
		return "D50";
	case ILMNT_D55:
		return "D55";
	case ILMNT_D60:
		return "D60";
	case ILMNT_D65:
		return "D65";
	case ILMNT_D75:
		return "D75";
	case ILMNT_E:
		return "E";
	case ILMNT_F2:
		return "F2";
	case ILMNT_F7:
		return "F7";
	case ILMNT_F11:
		return "F11";
	default:
		return "D65";
	}
}


OIlluminantID illuminantFromCSID(OCSID cs)
{
	switch(cs)
	{
	case CS_LINEAR_RGB:
	case CS_ADOBE_RGB_1998:
	case CS_APPLE_RGB:
	case CS_BRUCE_RGB:
	case CS_PAL_SECAM_RGB:
	case CS_SMPTE_C_RGB:
	case CS_sRGB:
	case CS_REC709:
	case CS_REC2020:
		return ILMNT_D65;
	case CS_BEST_RGB:
	case CS_BETA_RGB:
	case CS_COLORMATCH_RGB:
	case CS_DON_RGB_4:
	case CS_ECI_RGB_V2:
	case CS_EKTA_SPACE_PS5:
	case CS_PROPHOTO_RGB:
	case CS_WIDE_GAMUT_RGB:
		return ILMNT_D50;
	case CS_CIE_RGB:
	case CS_CIE_XYZ:
		return ILMNT_E;
	case CS_NTSC_RGB:
		return ILMNT_C;
	case CS_ALEXA_WIDE_GAMMUT_RGB:
	case CS_ACES_CG:
		return ILMNT_D60;
	default:
		return ILMNT_D65;
	}
}


//enum OGammaID {G_LINEAR, G_1p8, G_2p2, G_sRGB, G_2p4 };
OGammaID gammaFromCSID(OCSID cs)
{
	switch(cs)
	{
	case CS_LINEAR_RGB:
	case CS_CIE_XYZ:
		return G_LINEAR;
	case CS_ADOBE_RGB_1998:
		return G_2p2;
	case CS_APPLE_RGB:
		return G_1p8;
	case CS_BEST_RGB:
		return G_2p2;
	case CS_BETA_RGB:
		return G_2p2;
	case CS_BRUCE_RGB:
		return G_2p2;
	case CS_CIE_RGB:
		return G_2p2;
	case CS_COLORMATCH_RGB:
		return G_1p8;
	case CS_DON_RGB_4:
		return G_2p2;
	case CS_ECI_RGB_V2:
		return G_LSTAR;
	case CS_EKTA_SPACE_PS5:
		return G_2p2;
	case CS_NTSC_RGB:
		return G_2p2;
	case CS_PAL_SECAM_RGB:
		return G_2p2;
	case CS_PROPHOTO_RGB:
		return G_1p8;
	case CS_SMPTE_C_RGB:
		return G_2p2;
	case CS_sRGB:
		return G_sRGB;
	case CS_WIDE_GAMUT_RGB:
		return G_2p2;
	case CS_REC709:
		return G_REC709;
	case CS_REC2020:
		return G_REC2020;
	case CS_ALEXA_WIDE_GAMMUT_RGB:
		return G_2p2;
	case CS_ACES_CG:
		return G_2p2;
	default:
		return G_sRGB;
	}
}

//enum OGammaID {G_LINEAR, G_1p8, G_2p2, G_sRGB, G_2p4 };
const char* primariesID2text(OCSID cs)
{
	switch(cs)
	{
	case CS_LINEAR_RGB:
		return "XSI Linear RGB";
	case CS_ADOBE_RGB_1998:
		return "Adobe RGB 1998";
	case CS_APPLE_RGB:
		return "Apple RGB";
	case CS_BEST_RGB:
		return "Best RGB";
	case CS_BETA_RGB:
		return "Beta RGB";
	case CS_BRUCE_RGB:
		return "Bruce RGB";
	case CS_CIE_RGB:
		return "CIE RGB";
	case CS_COLORMATCH_RGB:
		return "ColorMatch RGB";
	case CS_DON_RGB_4:
		return "Don RGB 4";
	case CS_ECI_RGB_V2:
		return "ECI RGB v2"; // not sure
	case CS_EKTA_SPACE_PS5:
		return "EKTA space PS5";
	case CS_NTSC_RGB:
		return "NTSC RGB";
	case CS_PAL_SECAM_RGB:
		return "PAL/SECAM RGB";
	case CS_PROPHOTO_RGB:
		return "ProPhoto RGB";
	case CS_SMPTE_C_RGB:
		return "SMPTE C";
	case CS_sRGB:
		return "sRGB";
	case CS_WIDE_GAMUT_RGB:
		return "Wide Gamut RGB";
	case CS_REC709:
		return "Rec.709";
	case CS_REC2020:
		return "Rec.2020";
	case CS_CIE_XYZ:
		return "CIE XYZ";
	case CS_ALEXA_WIDE_GAMMUT_RGB:
		return "Alexa Wide Gammut RGB";
	case CS_ACES_CG:
		return "ACEScg";
	default:
		return "sRGB";
	}
}


// linear rgb to gamma rgb and vice versa
glm::vec3 applyGammaFromID(OGammaID gid, glm::vec3 rgb, bool invert = false)
{
	glm::vec3 ret;
	glm::vec3 overunder = rgb - glm::clamp(rgb,0.0f,1.0f);
	rgb -= overunder; //[0,1]

	switch(gid)
	{
	case G_LINEAR:
		ret = rgb;
		break;
	case G_1p8:
		if(invert)
			ret = glm::pow(rgb,glm::vec3(1.8f));
		else	
			ret = glm::pow(rgb,glm::vec3(1.0f/1.8f));
		break;
	case G_2p2 :
		if(invert)
			ret = glm::pow(rgb,glm::vec3(2.2f));
		else	
			ret = glm::pow(rgb,glm::vec3(1.0f/2.2f));
		break;
	case G_sRGB: // wiki
		if(invert)
		{
			for(int i=0; i<3; i++)
			{
				float a = glm::min(glm::max(0.0f,rgb[i]),1.0f);
				if(a <= 0.03928f)
					ret[i] = a/12.92f;
				else
					ret[i] = std::exp(2.4f * std::log((a + 0.055f) / 1.055f));
			}
		}
		else
		{
			for(int i=0; i<3; i++)
			{
				float a = glm::min(glm::max(0.0f,rgb[i]),1.0f);
				if(a <= 0.00304f)
					ret[i] = a*12.92f;
				else
					ret[i] = 1.055f*std::exp((1.0f/2.4f) * std::log(a))-0.055f;
			}
		}
		break;
	case G_2p4 :
		if(invert)
			ret = glm::pow(rgb,glm::vec3(2.4f));
		else	
			ret = glm::pow(rgb,glm::vec3(1.0f/2.4f));
		break;
	case G_REC709 : // wiki
		if(invert)
		{
			for(int i = 0; i<3; i++)
				if(rgb[i] < 0.081f)
					ret[i] = (rgb[i]/4.5f);
				else
					ret[i] = std::pow((rgb[i]+0.099f)/1.099f,1.0f/0.45f);
		}
		else
		{
			for(int i = 0; i<3; i++)
				if(rgb[i] < 0.018f)
					ret[i] = (4.5f*rgb[i]);
				else
					ret[i] = 1.099f*std::pow(rgb[i],0.45f)-0.099f;
		}
		break;
	case G_LSTAR:
		if(invert)
		{
			for(int i = 0; i<3; i++)
				if(rgb[i] <= 0.079996248f)
					ret[i] = (rgb[i]*100.0f/903.3f);
				else
					ret[i] = std::pow((rgb[i]+0.16f)/1.16f,3.0f);
		}
		else
		{
			for(int i = 0; i<3; i++)
				if(rgb[i] <= 0.008856f)
					ret[i] = (rgb[i]*903.3f/100.0f);
				else
					ret[i] = 1.16f*std::pow(rgb[i],1.0f/3.0f)-0.16f;
		}
		break;
	case G_REC2020:
		{
			float alpha = 1.0993f, beta = 0.0181f;
			if(invert)
			{
				for(int i = 0; i<3; i++)
					if(rgb[i] < 4.5f*beta)
						ret[i]/= 4.5f;
					else
						ret[i] = std::pow((rgb[i]+(alpha -1.0f)/alpha),1.0f/0.45f);
			}
			else
			{
				for(int i = 0; i<3; i++)
					if(rgb[i] <= beta)
						ret[i]*= 4.5f;
					else
						ret[i] = alpha*std::pow(rgb[i],0.45f)-(alpha-1.0f);
			}
			break;
		}
	default:
		ret = rgb;
		break;
	}

	return ret+overunder;
}

// linear rgb to gamma rgb and vice versa
const char* gammaID2text(OGammaID gid)
{
	switch(gid)
	{
	case G_LINEAR:
		return "Linear";
	case G_1p8:
		return "1.8";
	case G_2p2 :
		return "2.2";
	case G_sRGB: 
		return "sRGB";
	case G_2p4 :
		return "2.4";
	case G_REC709 :
		return "Rec.709";
	case G_LSTAR :
		return "L*";
	case G_REC2020:
		return "Rec.2020";
	default:
		return "sRGB";
	}
}


// From a colorspaceID, return the primaries 
// Rx Gx Bx
// Ry Gy By
// RY GY BY
glm::mat3 primariesFromCSID(OCSID cs)
{
	switch(cs)
	{
	case CS_LINEAR_RGB:
	case CS_sRGB:
	case CS_REC709:
		return glm::mat3(0.6400f,0.3300f,0.212656f,0.3000f,0.6000f,0.715158f,0.1500f,0.0600f,0.072186f); //ITU-R recommendation BT.709 ?
	case CS_ADOBE_RGB_1998:
		return glm::mat3(0.6400f,0.3300f,0.297361f,0.2100f,0.7100f,0.627355f,0.1500f,0.0600f,0.075285f);
	case CS_APPLE_RGB:
		return glm::mat3(0.6250f,0.3400f,0.244634f,0.2800f,0.5950f,0.672034f,0.1550f,0.0700f,0.083332);
	case CS_BEST_RGB:
		return glm::mat3(0.7347f,0.2653f,0.228457f,0.2150f,0.7750f,0.737352f,0.1300f,0.0350f,0.034191f);
	case CS_BETA_RGB:
		return glm::mat3(0.6888f,0.3112f,0.303273f,0.1986f,0.7551f,0.663786f,0.1265f,0.0352f,0.032941f);
	case CS_BRUCE_RGB:
		return glm::mat3(0.6400f,0.3300f,0.240995f,0.2800f,0.6500f,0.683554f,0.1500f,0.0600f,0.075452f);
	case CS_CIE_RGB:
	case CS_CIE_XYZ: // no need but need to return something
		return glm::mat3(0.7350f,0.2650f,0.176204f,0.2740f,0.7170f,0.812985f,0.1670f,0.0090f,0.010811f);
	case CS_COLORMATCH_RGB:
		return glm::mat3(0.6300f,0.3400f,0.274884f,0.2950f,0.6050f,0.658132f,0.1500f,0.0750f,0.066985f);
	case CS_DON_RGB_4:
		return glm::mat3(0.6960f,0.3000f,0.278350f,0.2150f,0.7650f,0.687970f,0.1300f,0.0350f,0.033680f);
	case CS_ECI_RGB_V2:
		return glm::mat3(0.6700f,0.3300f,0.320250f,0.2100f,0.7100f,0.602071f,0.1400f,0.0800f,0.077679f);
	case CS_EKTA_SPACE_PS5:
		return glm::mat3(0.6950f,0.3050f,0.260629f,0.2600f,0.7000f,0.734946f,0.1100f,0.0050f,0.004425f);
	case CS_NTSC_RGB:
		return glm::mat3(0.6700f,0.3300f,0.298839f,0.2100f,0.7100f,0.586811f,0.1400f,0.0800f,0.114350f);
	case CS_PAL_SECAM_RGB:
		return glm::mat3(0.6400f,0.3300f,0.222021f,0.2900f,0.6000f,0.706645f,0.1500f,0.0600f,0.071334f);
	case CS_PROPHOTO_RGB:
		return glm::mat3(0.7347f,0.2653f,0.288040f,0.1596f,0.8404f,0.711874f,0.0366f,0.0001f,0.000086f);
	case CS_SMPTE_C_RGB:
		return glm::mat3(0.6300f,0.3400f,0.212395f,0.3100f,0.5950f,0.701049f,0.1550f,0.0700f,0.086556f);
	case CS_WIDE_GAMUT_RGB:
		return glm::mat3(0.7350f,0.2650f,0.258187f,0.1150f,0.8260f,0.724938f,0.1570f,0.046f,0.0f);
	case CS_REC2020:
		return glm::mat3(0.708f,0.292f,0.0f, 0.170f,0.797f,0.0f, 0.131f,0.0180f,0.0);
	case CS_ALEXA_WIDE_GAMMUT_RGB:
		return glm::mat3(0.6840f,0.3130f,0.291954f,0.2210f,0.8480f,0.823841f,0.0861f,-0.102f,-0.115795f);
	case CS_ACES_CG:
		return glm::mat3(0.713f,0.293f,0.27222872f,0.165f,0.83f,0.67408177f,0.128f,0.044f,0.05368952f);
	default:
		return glm::mat3(0.6400f,0.3300f,0.212656f,0.3000f,0.6000f,0.715158f,0.1500f,0.0600f,0.072186f);
	}
}





// ColorSpace Information Constants : [name, gamma ID, reference white ID, Red Primaries (xyY), Green Primaries (xyY), Blue Primaries (xyY)]  {}

// A class to convert between colorspace and 
class OColorspace
{
public:
	// Constructor defaults to sRGB :  	 	 	 	 	 	 	 	
	OColorspace(OGammaID _gId = G_2p2, OIlluminantID _ilmntId = ILMNT_D65, 
		float _Rx = 0.6400f, float _Ry = 0.3300f, float _RY = 0.212656f, 
		float _Gx = 0.3000f, float _Gy = 0.6000f, float _GY = 0.715158f,
		float _Bx = 0.1500f, float _By = 0.0600f, float _BY = 0.072186f)
		:
		gId(_gId),
		ilmntId(_ilmntId),
		Rx(_Rx), Ry(_Ry), RY(_Ry), 
		Gx(_Gx), Gy(_Gy), GY(_GY),
		Bx(_Bx), By(_By), BY(_BY),
		hasPrimariesInfo(false)
	{
		illuminantID2XYZ(_ilmntId, Xw, Yw, Zw);
		updateMatrices();
	}

	OColorspace(OGammaID _gId, OIlluminantID _ilmntId,	glm::mat3 _primaries)
		:
		gId(_gId),
		ilmntId(_ilmntId),
		hasPrimariesInfo(false),
		Rx(_primaries[0][0]), Ry(_primaries[0][1]), RY(_primaries[0][2]), 
		Gx(_primaries[1][0]), Gy(_primaries[1][1]), GY(_primaries[1][2]),
		Bx(_primaries[2][0]), By(_primaries[2][1]), BY(_primaries[2][2])
	{
		illuminantID2XYZ(_ilmntId, Xw, Yw, Zw);
		updateMatrices();
	}

	OColorspace(OGammaID _gId, OIlluminantID _ilmntId,	OCSID _primariesId)
		:
		gId(_gId),
		ilmntId(_ilmntId),
		primariesId(_primariesId),
		hasPrimariesInfo(true)
	{
		illuminantID2XYZ(_ilmntId, Xw, Yw, Zw);
		glm::mat3 tmp = primariesFromCSID(_primariesId);
		Rx = tmp[0][0];
		Ry = tmp[0][1];
		RY = tmp[0][2];
		Gx = tmp[1][0];
		Gy = tmp[1][1];
		GY = tmp[1][2];
		Bx = tmp[2][0];
		By = tmp[2][1];
		BY = tmp[2][2];
		updateMatrices();
	}

	// Returns the 3x3 matrix to change the white point using the Bradford method
	glm::mat3 WsToWdMatrix(OColorspace &dest)
	{
		glm::vec3 Ws(Xw, Yw, Zw), Wd(dest.Xw, dest.Yw, dest.Zw);

		glm::vec3 PYBs = c_BradfordMatrix*Ws;
		glm::vec3 PYBd = c_BradfordMatrix*Wd;
		glm::mat3 PYBdoverPYBs(PYBd[0]/PYBs[0],0.0f,0.0f,0.0f,PYBd[1]/PYBs[1],0.0f,0.0f,0.0f,PYBd[2]/PYBs[2]); // I*(PYBd/PYBs)

		return c_BradfordMatrixI*PYBdoverPYBs*c_BradfordMatrix;
	}

	// Getters
	const glm::mat3& getXYZtoRGBmatrix(){ return XYZtoRGBmat; }
	const glm::mat3& getRGBtoXYZmatrix(){ return RGBtoXYZmat; }
	std::string getInfo()
	{
		char buf[256];
		std::sprintf(buf,"Primaries = %s, Gamma = %s, Illuminant = %s", (hasPrimariesInfo?primariesID2text(primariesId):"?"), gammaID2text(gId),illuminantID2text(ilmntId));
		return std::string(buf);
	}

	// Apply gamma or DeGamma
	glm::vec3 applyGamma(glm::vec3 RGB, bool invert = false)
	{
		return applyGammaFromID(gId,RGB,invert);
	}

	// Conversion
	glm::vec3 RGBtoXYZ(glm::vec3 RGB)
	{
		glm::vec3 linRGB = applyGammaFromID(gId,RGB,true);
		return RGBtoXYZmat*linRGB;
	}

	glm::vec3 XYZtoRGB(glm::vec3 XYZ)
	{ 
		glm::vec3 linRGB = XYZtoRGBmat*XYZ;
		return applyGammaFromID(gId, linRGB, false);
	}

private:
	// must be called in the constructor
	void updateMatrices()
	{
		if(primariesId == CS_CIE_XYZ) // special case
		{
			XYZtoRGBmat = glm::mat3();
			RGBtoXYZmat = glm::mat3();
		}
		else
		{
			float Xr = Rx/Ry;
			float Yr = 1.0f;
			float Zr = (1.0f-Rx-Ry)/Ry;
			float Xg = Gx/Gy;
			float Yg = 1.0f;
			float Zg = (1.0f-Gx-Gy)/Gy;
			float Xb = Bx/By;
			float Yb = 1.0f;
			float Zb = (1.0f-Bx-By)/By;
			glm::mat3 XYZrgb(Xr,Yr,Zr,Xg,Yg,Zg,Xb,Yb,Zb);
			glm::vec3 S = glm::inverse(XYZrgb)*glm::vec3(Xw,Yw,Zw);
			RGBtoXYZmat = glm::mat3(S[0]*XYZrgb[0][0], S[0]*XYZrgb[0][1], S[0]*XYZrgb[0][2], S[1]*XYZrgb[1][0], S[1]*XYZrgb[1][1], S[1]*XYZrgb[1][2],S[2]*XYZrgb[2][0], S[2]*XYZrgb[2][1], S[2]*XYZrgb[2][2]);
			XYZtoRGBmat = glm::inverse(RGBtoXYZmat);
		}
	}

	// members
	OGammaID gId;
	OIlluminantID ilmntId;
	OCSID primariesId;
	bool hasPrimariesInfo;
	float Xw, Yw, Zw;
	float Rx, Ry, RY;
	float Gx, Gy, GY;
	float Bx, By, BY;
	glm::mat3 RGBtoXYZmat;
	glm::mat3 XYZtoRGBmat;

};


class OColorspaceConverter
{
public:

	// Constructors
	OColorspaceConverter(bool _switchInOut = false, bool _useBradford = false){switchInOut = false; useBradford=false;}

	OColorspaceConverter(OColorspace _cs_in, OColorspace _cs_out, bool _switchInOut = false, bool _useBradford = false):
		switchInOut(_switchInOut), 
		useBradford(_useBradford)
	{
		cs_In=_cs_in;
		cs_Out=_cs_out;

		updateConversionMatrix();
	}

	OColorspaceConverter(OGammaID in_gId, OIlluminantID in_ilmntId, OCSID in_csPrimariesID, OGammaID out_gId, OIlluminantID out_ilmntId, OCSID out_csPrimariesID, bool _switchInOut = false, bool _useBradford = false ):
		switchInOut(_switchInOut), 
		useBradford(_useBradford)
	{
		cs_In  = OColorspace(in_gId, in_ilmntId, in_csPrimariesID);
		cs_Out = OColorspace(out_gId,out_ilmntId,out_csPrimariesID);
		updateConversionMatrix();
	}

	OColorspaceConverter(	OGammaID in_gId,  OIlluminantID in_ilmntId,  float in_Rx,  float in_Ry,  float in_RY,  float in_Gx,  float in_Gy,  float in_GY,  float in_Bx,  float in_By,  float in_BY,
							OGammaID out_gId, OIlluminantID out_ilmntId, float out_Rx, float out_Ry, float out_RY, float out_Gx, float out_Gy, float out_GY, float out_Bx, float out_By, float out_BY, 
							bool _switchInOut = false, bool _useBradford = false ):
		switchInOut(_switchInOut), 
		useBradford(_useBradford)
	{
		// create color spaces
		cs_In  = OColorspace(in_gId,  in_ilmntId, in_Rx, in_Ry, in_RY, in_Gx, in_Gy, in_GY, in_Bx, in_By, in_BY);
		cs_Out = OColorspace(out_gId,  out_ilmntId, out_Rx, out_Ry, out_RY, out_Gx, out_Gy, out_GY, out_Bx, out_By, out_BY);
		updateConversionMatrix();
	}
	
	//free memory
	~OColorspaceConverter(){;}


	// Setters
	void setIn(OGammaID in_gId,  OIlluminantID in_ilmntId,  OCSID in_primariesId)
	{
		cs_In  = OColorspace(in_gId,  in_ilmntId, in_primariesId);
		updateConversionMatrix();
	}

	void setOut(OGammaID out_gId,  OIlluminantID out_ilmntId,  OCSID out_primariesId)
	{
		cs_Out  = OColorspace(out_gId, out_ilmntId, out_primariesId);
		updateConversionMatrix();
	}

	void setSwitchInOut(bool _switchInOut)
	{
		switchInOut = _switchInOut;
		updateConversionMatrix();
	}

	void setUseBradford(bool _useBradford)
	{
		useBradford = _useBradford;
		updateConversionMatrix();
	}

	// Getters
	const OColorspace& getCSIn(){return cs_In;}
	const OColorspace& getCSOut(){return cs_Out;}

	// Converts from RGB (CS_IN) -> RGB (CS_OUT) or vice-versa with or without chromatic adaptation
	glm::vec3 convertRGB(float r, float g, float b)
	{
		if(switchInOut)
			return cs_In.applyGamma(conversionMatrix*cs_Out.applyGamma(glm::vec3(r,g,b),true));
		else
			return cs_Out.applyGamma(conversionMatrix*cs_In.applyGamma(glm::vec3(r,g,b),true));
	}

	std::string getInfo()
	{
		char buf[512];
		std::sprintf(buf,"Info : [ In : %s ] and [ Out : %s ] ",cs_In.getInfo().c_str(), cs_Out.getInfo().c_str());
		return std::string(buf);

	}

	// return the adapted XYZtoRGB matrix
	const glm::mat3 getConversionMatrix() const	{ return conversionMatrix;	}

private:

	void updateConversionMatrix()
	{

		if(switchInOut)
		{
			if(useBradford)
				conversionMatrix = cs_In.getXYZtoRGBmatrix()*cs_Out.WsToWdMatrix(cs_In)*cs_Out.getRGBtoXYZmatrix();
			else
				conversionMatrix = cs_In.getXYZtoRGBmatrix()*cs_Out.getRGBtoXYZmatrix();

		}
		else
		{
			if(useBradford)
				conversionMatrix = cs_Out.getXYZtoRGBmatrix()*cs_In.WsToWdMatrix(cs_Out)*cs_In.getRGBtoXYZmatrix();
			else
				conversionMatrix = cs_Out.getXYZtoRGBmatrix()*cs_In.getRGBtoXYZmatrix();
		}
	}

	OColorspace cs_In;
	OColorspace cs_Out;
	bool switchInOut;
	bool useBradford;
	glm::mat3 conversionMatrix;
};

