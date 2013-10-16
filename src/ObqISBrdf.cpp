#include "ObqISBrdf.h"

void ObqISBrdf::initializeBrdf(AtUInt32 resolution){

	// Initialize resolution
	m_res = resolution;
	m_resf = static_cast<float>(m_res);

	// common res
	m_resM1 = m_res-1;
	m_resM1f = static_cast<float>(m_resM1);
	m_2xRes = 2*m_res-1;
	m_2xResf =static_cast<float>(m_2xRes);
	m_2xResM1 = m_2xRes-1;
	m_2xResM1f = static_cast<float>(m_2xResM1);

	// iso
	m_toDpRes = m_res*m_2xRes;
	m_toDpResf = static_cast<float>(m_toDpRes);

	//allocation
	m_data = new AtColor[m_res*m_res*m_2xRes];
	m_solidAngles = new float[m_res];

	AtUInt32 tiRes = m_res, toRes = m_res, dpRes = m_2xRes;

	/////////////////
	//-- SOLID ANGLES
	/////////////////
	double totalOmega = 0.0;

	for(AtUInt32 j = 0; j < toRes; j++){

		double omega = 0.0;

		// top
		if(j==0)
		{
			double t = AI_PIOVER2/static_cast<double>(toRes-1);
			double ph = AI_PI/static_cast<double>(dpRes-1);
			glm::dvec3 a(0.0,1.0,0.0);
			glm::dvec3 b(glm::sin(t/2.0),glm::cos(t/2.0),0.0);
			glm::dvec3 c = rotateVectorAroundY(b,ph/2.0);
			omega = 2.0*static_cast<double>(dpRes-1)*2.0*getSolidAngleOfTriangle(a, b, c);

		}
		// bottom
		else if(j == toRes-1)
		{

			double t = AI_PIOVER2/static_cast<double>(toRes-1);
			double jt = static_cast<double>(j)*t;
			double tover2 = t/2.0;
			double ph = AI_PI/static_cast<double>(dpRes-1);
			double phover2 = ph/2.0;
			glm::dvec3 a(glm::sin(jt),glm::cos(jt),0.0);
			glm::dvec3 d = rotateVectorAroundY(a,phover2);
			glm::dvec3 b(glm::sin(jt-tover2),glm::cos(jt-tover2),0.0);
			glm::dvec3 c = rotateVectorAroundY(b,phover2);

			omega = 2.0*(getSolidAngleOfTriangle(a, b, c) + getSolidAngleOfTriangle(a, c, d));

		}
		// rest
		else
		{

			double t = AI_PIOVER2/static_cast<double>(toRes-1);
			double jt = static_cast<double>(j)*t;
			double tover2 = t/2.0;
			double ph = AI_PI/static_cast<double>(dpRes-1);
			double phover2 = ph/2.0;
			glm::dvec3 a(glm::sin(jt),glm::cos(jt),0.0);
			glm::dvec3 d = rotateVectorAroundY(a,phover2);
			glm::dvec3 b(glm::sin(jt-tover2),glm::cos(jt-tover2),0.0);
			glm::dvec3 c = rotateVectorAroundY(b,phover2);
			glm::dvec3 e(glm::sin(jt+tover2),glm::cos(jt+tover2),0.0);
			glm::dvec3 f = rotateVectorAroundY(e,phover2);

			omega = 2.0*(getSolidAngleOfTriangle(a, b, c) + getSolidAngleOfTriangle(a, c, d) + getSolidAngleOfTriangle(a, d, e) + getSolidAngleOfTriangle(d, f, e));

		}

		///////////////////////
		// STORE SOLID ANGLES

		if(j==0)	// TOP
		{
			m_solidAngles[j] = static_cast<float>(omega);
			totalOmega+=omega;
		}
		else if(j == toRes-1) // bottom
		{

			m_solidAngles[j] = static_cast<float>(omega);
			totalOmega+=omega*2.0*(m_2xResf-1);
		}
		else
		{
			m_solidAngles[j] = static_cast<float>(omega);
			totalOmega+=omega*2.0*(m_2xResf-1);

		}
	}

	m_init = true;
}

void ObqISBrdf::initializeIsBrdf(AtUInt32 resolution){

	// Initialize resolution
	m_isRes = resolution;
	m_isResf = static_cast<float>(m_isRes);

	// common res
	m_isResM1 = m_isRes-1;
	m_isResM1f = static_cast<float>(m_isResM1);
	m_is2xRes = 2*m_isRes-1;
	m_is2xResf =static_cast<float>(m_is2xRes);
	m_is2xResM1 = m_is2xRes-1;
	m_is2xResM1f = static_cast<float>(m_is2xResM1);

	// iso
	m_isToDpRes = m_isRes*m_is2xRes;
	m_isToDpResf = static_cast<float>(m_isToDpRes);

	// allocate
	m_isSolidAngles = new float[m_isRes];
	m_isSumPdf = new double[m_isRes];
	m_half_GoE = new AtUInt32[m_isRes];
	m_half_L = new AtUInt32[m_isRes];	
	m_isDirRes = (1 + m_isResM1*m_is2xRes);
	m_isData = new ObqISBin[m_isRes*m_isDirRes];


	AtUInt32 tiRes = m_isRes, toRes = m_isRes, dpRes = m_is2xRes;

	/////////////////
	//-- SOLID ANGLES
	/////////////////
	double totalOmega = 0.0;

	for(AtUInt32 j = 0; j < toRes; j++){

		double omega = 0.0;

		// top
		if(j==0)
		{
			double t = AI_PIOVER2/static_cast<double>(toRes-1);
			double ph = AI_PI/static_cast<double>(dpRes-1);
			glm::dvec3 a(0.0,1.0,0.0);
			glm::dvec3 b(glm::sin(t/2.0),glm::cos(t/2.0),0.0);
			glm::dvec3 c = rotateVectorAroundY(b,ph/2.0);
			omega = 2.0*static_cast<double>(dpRes-1)*2.0*getSolidAngleOfTriangle(a, b, c);

		}
		// bottom
		else if(j == toRes-1)
		{

			double t = AI_PIOVER2/static_cast<double>(toRes-1);
			double jt = static_cast<double>(j)*t;
			double tover2 = t/2.0;
			double ph = AI_PI/static_cast<double>(dpRes-1);
			double phover2 = ph/2.0;
			glm::dvec3 a(glm::sin(jt),glm::cos(jt),0.0);
			glm::dvec3 d = rotateVectorAroundY(a,phover2);
			glm::dvec3 b(glm::sin(jt-tover2),glm::cos(jt-tover2),0.0);
			glm::dvec3 c = rotateVectorAroundY(b,phover2);

			omega = 2.0*(getSolidAngleOfTriangle(a, b, c) + getSolidAngleOfTriangle(a, c, d));

		}
		// rest
		else
		{

			double t = AI_PIOVER2/static_cast<double>(toRes-1);
			double jt = static_cast<double>(j)*t;
			double tover2 = t/2.0;
			double ph = AI_PI/static_cast<double>(dpRes-1);
			double phover2 = ph/2.0;
			glm::dvec3 a(glm::sin(jt),glm::cos(jt),0.0);
			glm::dvec3 d = rotateVectorAroundY(a,phover2);
			glm::dvec3 b(glm::sin(jt-tover2),glm::cos(jt-tover2),0.0);
			glm::dvec3 c = rotateVectorAroundY(b,phover2);
			glm::dvec3 e(glm::sin(jt+tover2),glm::cos(jt+tover2),0.0);
			glm::dvec3 f = rotateVectorAroundY(e,phover2);

			omega = 2.0*(getSolidAngleOfTriangle(a, b, c) + getSolidAngleOfTriangle(a, c, d) + getSolidAngleOfTriangle(a, d, e) + getSolidAngleOfTriangle(d, f, e));

		}

		///////////////////////
		// STORE SOLID ANGLES

		if(j==0)	// TOP
		{
			m_isSolidAngles[j] = static_cast<float>(omega);
			totalOmega+=omega;
		}
		else if(j == toRes-1) // bottom
		{

			m_isSolidAngles[j] = static_cast<float>(omega);
			totalOmega+=omega*2.0*(m_is2xResf-1);
		}
		else
		{
			m_isSolidAngles[j] = static_cast<float>(omega);
			totalOmega+=omega*2.0*(m_is2xResf-1);

		}
	}

	m_isInit = true;
}

bool ObqISBrdf::readMSKBrdf(const char* filename)
{

	FILE *f = fopen(filename, "rb");
	if (!f){
		AiMsgError("!E! Not a valid filename : %s", filename);
		return false;
	}
	else
	{

		AtUInt32 dims[4];
		fread(dims, sizeof(AtUInt32), 4, f);

		int n = dims[0] * dims[1] * dims[2];
		AiMsgInfo("Read_MSKBrdf : dims = %u, %u, %u and %u channels.",dims[0] , dims[1] , dims[2] , dims[3]);
		// transfer to custom brdf

		// allocate brdf
		destroyData();
		initializeBrdf(dims[0]);		

		glm::dvec3* brdf = new glm::dvec3[n];
		fread(brdf, sizeof(glm::dvec3), n, f);
		fclose(f);

		for(AtUInt32 i = 0; i < dims[0]; i++)
		{
			for(AtUInt32 j = 0; j < dims[1]; j++)
			{
				for(AtUInt32 k = 0; k < dims[2]; k++)
				{
					setDataFromIndices(i,j,k,brdf[ i*dims[1]*dims[2] + j*dims[2] + k]);
				}
			}
		}
		delete[] brdf;

		return true;
	}
}

bool ObqISBrdf::readRegIso(const char* filename)
{

    double *brdf;
	int dims[3];
    bool ret = read_brdf(filename,brdf,dims);

	if(!ret)
		return false;
    // transfer to custom brdf
    // allocate brdf
		destroyData();
		initializeBrdf(91);

    for(AtUInt32 i = 0; i < m_res; i++){
         double theta_in= static_cast<double>(i)/static_cast<double>(m_res-1)*AI_PIOVER2;
                 double fi_in = 0.0;
        for(AtUInt32 j = 0; j < m_res; j++){
             double theta_out = static_cast<double>(j)/static_cast<double>(m_res-1)*AI_PIOVER2;
            for(AtUInt32 k = 0; k < m_2xRes; k++)
            {
                double fi_out = static_cast<double>(k)/static_cast<double>(m_2xRes-1)*AI_PI;
                double red_val, green_val, blue_val;
                lookup_brdf_val(brdf, theta_in, fi_in,theta_out, fi_out,red_val,green_val,blue_val);
                setDataFromIndices(i,j,k,glm::dvec3(red_val,green_val,blue_val));
            }
        }
    }
    delete[] brdf;
	return true;
}

bool ObqISBrdf::createIS(AtUInt32 isRes)
{
	// create isRes data and fetch color and solid angle
	initializeIsBrdf(isRes);

	// first pass : pdf value = luma color * solid angle
	for(AtUInt32 isTo = 0; isTo < m_isRes; isTo++)
	{
		float theta_o = getAngleFromIndex(isTo,m_isResM1f,c_PiOver2);
		std::vector<ObqISBin> bins;
		bins.reserve(m_isDirRes);
		m_isSumPdf[isTo] = 0.0;
		float total_sa = 0.0f;
		for(AtUInt32 isTi = 0; isTi < m_isRes; isTi++)
		{

			float theta_i = getAngleFromIndex(isTi,m_isResM1f,c_PiOver2);
			float solidAngle = m_isSolidAngles[isTi];

			// North Pole is only one direction
			if(isTi==0)
			{
				AtColor rgb = getDataInterpolated(theta_i,theta_o,0.0f);
				double pdf = getPdfValueOfColor(rgb);
				bins.push_back(ObqISBin(theta_i,0.0f,pdf,0.0,solidAngle));
			}
			else
			{
				for(AtUInt32 isDp = 0; isDp < m_is2xRes; isDp++)
				{
					float phi = getAngleFromIndex(isDp,m_is2xResM1f,c_Pi);
					AtColor rgb = getDataInterpolated(theta_i,theta_o,phi);
					double solid = ((isDp==0 || isDp == m_is2xResM1)?solidAngle:solidAngle*2.0);
					double pdf = getPdfValueOfColor(rgb);
					
					bins.push_back(ObqISBin(theta_i,phi,pdf,0.0,solid));

				}
			}

		}

		AtUInt32 index = (isTo+1)*m_isDirRes-1;
		std::sort(bins.begin(),bins.end());

		// reverse order -> decreasing
		for (std::vector<ObqISBin>::iterator it = bins.begin(); it != bins.end(); ++it)
		{
			m_isData[index] = (*it);
			m_isSumPdf[isTo]+=(m_isData[index].pdf*m_isData[index].solid);
			--index;
		}


		// second pass, all pdfs / sum pdf (normalization), create cdf values (and note where is .5 ? OPTIONAL)
		double currentCdf = 0.0;
		bool halfInit = false;
		m_half_L[isTo] = m_isDirRes -1;
		m_half_GoE[isTo] = 0;
		AtUInt32 baseIndex = isTo*m_isDirRes;
		for(AtUInt32 i = 0; i < m_isDirRes; ++i)
		{
			
			if(m_useHalfs && !halfInit && currentCdf >= 0.5)
			{
				m_half_L[isTo] = i;
				m_half_GoE[isTo] = i-1;
				halfInit =  true;
			}

			m_isData[baseIndex+i].pdf/=m_isSumPdf[isTo];
			m_isData[baseIndex+i].cdf = currentCdf;
			currentCdf += m_isData[baseIndex+i].pdf*m_isData[baseIndex+i].solid;
		}

	}

	AiMsgInfo("Importance sampling data creation completed.");
	return true;
}