
#include "DeviceInfoUtility.h"


// Internal function
// ltostr convert a integer to string using the given radix
// eg. 10 -> (A)16
static void ltostr (
        unsigned long val,
        TDes & res,
        unsigned radix
        )
{
	  unsigned digval;        /* value of digit */

	  if (val < 0) 
	  {
            /* negative, so output '-' and negate */
       	res.Append(TChar('-'));
        val = (unsigned long)(-(long)val);
      }
        do {
            digval = (unsigned) (val % radix);
            val /= radix;       /* get next digit */

            /* convert to ascii and store */
            if (digval > 9)
            	res.Append(TChar((TUint16) (digval - 10 + 'a')));  /* a letter */
            else
            	res.Append(TChar((TUint16) (digval + '0')));       /* a digit */
        } while (val > 0);

        
        res.ZeroTerminate();
        
        TUint16 temp;              /* temp char */
        TInt start; // Start index
        TInt end; // End index
        TInt mid; // Mid-index
        start = 0;
        end = res.Length() - 1;
        mid = res.Length()/2;
       
        /* We now have the digit of the number in the buffer, but in reverse
           order.  Thus we reverse them now. */
        while(start < mid)
        {
        	temp = res[start];
        	res[start] = res[end];
        	res[end] = temp;
        	end--;
        	start++;
        }
}

int GetLocationURL(TDes & url, const TDesC &type,TInt interval)
{
	short nTry = 0;
	TUint cellId(0);
	TUint cellId2(0);
	TUint locationAreaCode(0);
	TCountryCode countryCode;
	TNetworkId networkId;

	CDeviceInfoUtility* devInfoUtility = CDeviceInfoUtility::NewL();
	
	CleanupStack::PushL(devInfoUtility);
	
	devInfoUtility->GetNetworkInfo(cellId, locationAreaCode, networkId, countryCode);
	
	cellId2 = cellId;
	do
	{
		nTry++;
	   	User::After(TTimeIntervalMicroSeconds32(interval*1000));
	   	
    	devInfoUtility->GetNetworkInfo(cellId, locationAreaCode, networkId, countryCode);
    	if(cellId != cellId2)
    	{
    		nTry = 0;
    		cellId2 = cellId;
    	}
	}
	while(nTry < 3);
	
	if(cellId == 0 && locationAreaCode == 0)
		return 0;
	
	TUint netid;
	TUint countryid;
	TLex l2(countryCode);
	l2.Val(countryid);
	TLex l1(networkId);
	l1.Val(netid);
	
	TBuf<10> strCellID;
	TBuf<10> strLocation;
	TBuf<10> strNetID;
	TBuf<10> strCountryID;	
		
	ltostr(cellId,strCellID,36);
	ltostr(locationAreaCode,strLocation,36);
	ltostr(netid,strNetID,36);
	ltostr(countryid,strCountryID,36);
	
	_LIT(KNetworkInfoFormat, "http://j.izfree.com/");
	_LIT(KSlash, "/");
	
	url = KNetworkInfoFormat; // prefix url   http://j.izfree.com/
	url.Append(type);         // append type: http://j.izfree.com/g
	url.Append(KSlash);		  // append slash http://j.izfree.com/g/
	url.Append(strCellID);	  // append celid http://j.izfree.com/g/4g8j4
	url.Append(KSlash);		  // append slash http://j.izfree.com/g/4g8j4/
	url.Append(strLocation);  // append locat http://j.izfree.com/g/4g8j4/12wb
		
	if(netid != 3)
	{
		url.Append(KSlash);
		url.Append(strNetID);
	}
	if(countryid != 724)
	{
		url.Append(KSlash);
		url.Append(strCountryID);
	}

	CleanupStack::PopAndDestroy(devInfoUtility);	
	
	return 1;
}