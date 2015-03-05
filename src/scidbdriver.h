/*
Copyright (c) 2015 Marius Appel <marius.appel@uni-muenster.de>

This file is part of scidb4gdal. scidb4gdal is licensed under the MIT license.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
-----------------------------------------------------------------------------*/

#ifndef SCIDB_DRIVER_H
#define SCIDB_DRIVER_H

#include <iostream>
#include "utils.h"
#include "shimclient.h"

namespace scidb4gdal
{


    class SciDBRasterBand;
    class SciDBDataset;


    /**
     * GDALDataset subclass implementing core GDAL functionality
     */
    class SciDBDataset : public GDALPamDataset
    {
        friend class SciDBRasterBand;

        SciDBSpatialArray _array; //!< associated array metadata object
        ShimClient *_client; //!< associated shim client metadata object

    public:
        /**
	 * Default constructor for creating SciDBDataset instance for a given connectionstring
	 * @param connstr string representation of a connection string, e.g. "SCIDB:array=<arrayname> [host=<host> port=<port> user=<user> password=<password>]"
	 */
        SciDBDataset ( const string &connstr );
	
	/**
	 * Destructor for SciDBDatasets
	 */
        ~SciDBDataset();

	/**
	 * Function called by GDAL once a SciDB dataset is requested
	 * @see GDALDataset::Open
	 */
        static GDALDataset *Open ( GDALOpenInfo *poOpenInfo );
	
	/**
	 * Decides whether a dataset is a SciDB dataset or not, depends on the connection string prefix SCIDB:
	 */
        static int Identify ( GDALOpenInfo *poOpenInfo );

	/**
	 * Returns a pointer to the shim client object
	 */
        ShimClient *getClient() {
            return _client;
        }

        /**
	 * Returns affine transformation parameters
	 */
        CPLErr GetGeoTransform ( double *padfTransform );
	
	/**
	 * Returns WKT spatial reference string
	 */
        const char *GetProjectionRef();
    };


    
    
    
     /**
     * GDALRasterBand subclass implementing core GDAL functionality for single bands
     */
    class SciDBRasterBand : public GDALPamRasterBand
    {
        friend class SciDBDataset;

        SciDBSpatialArray *_array; //!< associated array metadata object

    public:

	/**
	 * Default constructor for SciDB attribute bands
	 */
        SciDBRasterBand ( SciDBDataset *poDS, SciDBSpatialArray *array, int nBand );
	
	/**
	 * Band destructor
	 */
        ~SciDBRasterBand();


	/**
	 * GDAL function called as array attribtue data is requested, loads data from SciDB server and might take some time thus
	 */
        virtual CPLErr IReadBlock ( int nBlockXOff, int nBlockYOff, void *pImage );
    };



}


#endif