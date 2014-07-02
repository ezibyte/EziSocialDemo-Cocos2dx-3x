/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package mobi.rjg.underfire;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;

import com.ezibyte.social.EziSocialManager;

public class AppActivity extends Cocos2dxActivity
{

	protected void onCreate(Bundle savedInstanceState)

	{
		super.onCreate(savedInstanceState);
		try
		{
			PackageInfo info = getPackageManager().getPackageInfo("mobi.rjg.underfire",
					PackageManager.GET_SIGNATURES);
			for (Signature signature : info.signatures)
			{
				MessageDigest md = MessageDigest.getInstance("SHA");
				md.update(signature.toByteArray());
				Log.e("MY KEY HASH:", Base64.encodeToString(md.digest(), Base64.DEFAULT)); // This
																							// is
																							// if
																							// you
																							// want
																							// to
																							// know
																							// your
																							// app
																							// hash
																							// key.
			}
		}
		catch (NameNotFoundException e)
		{
		}
		catch (NoSuchAlgorithmException e)
		{
		}
		EziSocialManager.initWithActivity(this, "1426090124326694", true,
				savedInstanceState);

	}

	@Override
	protected void onResume()
	{
		super.onResume();
		EziSocialManager.applicationResumed();
	}

	@Override
	public void onPause()
	{
		super.onPause();
		EziSocialManager.applicationPaused();
	}

	@Override
	public void onDestroy()
	{
		super.onDestroy();
		EziSocialManager.applicationDestroyed();
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		super.onActivityResult(requestCode, resultCode, data);
		EziSocialManager.onActivityResult(requestCode, resultCode, data);
	}
}
