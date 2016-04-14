/* file: covariance_csr_online.cpp */
/*******************************************************************************
* Copyright 2014-2016 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/*
!  Content:
!    C++ example of variance-covariance matrix computation in the online
!    processing mode
!******************************************************************************/

/**
 * <a name="DAAL-EXAMPLE-CPP-COVARIANCE_CSR_ONLINE"></a>
 * \example covariance_csr_online.cpp
 */


#include "daal.h"
#include "service.h"

using namespace std;
using namespace daal;
using namespace daal::algorithms;

typedef float  dataFPType;          /* Data floating-point type */
typedef double algorithmFPType;     /* Algorithm floating-point type */

/* Input data set parameters */
const size_t nBlocks          = 4;

const string datasetFileNames[] =
{
    "../data/online/covcormoments_csr_1.csv",
    "../data/online/covcormoments_csr_2.csv",
    "../data/online/covcormoments_csr_3.csv",
    "../data/online/covcormoments_csr_4.csv"
};

int main(int argc, char *argv[])
{
    checkArguments(argc, argv, 4, &datasetFileNames[0], &datasetFileNames[1], &datasetFileNames[2], &datasetFileNames[3]);

    /* Create an algorithm to compute a variance-covariance matrix in the online processing mode using the default method */
    covariance::Online<algorithmFPType, covariance::fastCSR> algorithm;

    for(size_t i = 0; i < nBlocks; i++)
    {
        CSRNumericTable *dataTable = createSparseTable<dataFPType>(datasetFileNames[i]);

        /* Set input objects for the algorithm */
        algorithm.input.set(covariance::data, services::SharedPtr<CSRNumericTable>(dataTable));

        /* Compute partial estimates */
        algorithm.compute();
    }

    /* Finalize the result in the online processing mode */
    algorithm.finalizeCompute();

    /* Get the computed variance-covariance matrix */
    services::SharedPtr<covariance::Result> res = algorithm.getResult();

    printNumericTable(res->get(covariance::covariance), "Covariance matrix (upper left square 10*10) :", 10, 10);
    printNumericTable(res->get(covariance::mean),       "Mean vector:", 1, 10);

    return 0;
}