﻿using System;

using f2i.energisticsStandardsApi;
using f2i.energisticsStandardsApi.common;
using f2i.energisticsStandardsApi.resqml2_0_1;

// Add the fesapi cpp and hdf5, szip, zlib dll into the executable directory.

namespace example
{
    static class Program
    {
        private static void serialize()
        {
            using (EpcDocument epc_file = new EpcDocument("../../../TestingPackageCs"))
            {
                LocalDepth3dCrs crs = epc_file.createLocalDepth3dCrs(Guid.NewGuid().ToString(), "UTF8 Crs title : éàç : олег1", 0.0, 0.0, 0.0, 0.0, eml20__LengthUom.eml20__LengthUom__m, 5215, eml20__LengthUom.eml20__LengthUom__m, "Unknown", false);
                System.Console.WriteLine("Serialize : CRS title is " + crs.getTitle());
                epc_file.serialize();
                epc_file.close();
            }
        }

        private static void deserialize()
        {
            using (EpcDocument epc_file = new EpcDocument("../../../TestingPackageCs"))
            {
                string status = epc_file.deserialize();
                if (status.Length > 0)
                    System.Console.WriteLine(status);
                LocalDepth3dCrsVector crs_set = epc_file.getLocalDepth3dCrsSet();
                System.Console.WriteLine("Deserialize : CRS title is " + crs_set[0].getTitle());
            }
        }

        static void Main(string[] args)
        {
            serialize();
            deserialize();
        }
    }
}
