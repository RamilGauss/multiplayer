/*!
	@file
	@author		Georgiy Evmenov
	@date		11/2007
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_I_CROPPED_RECTANGLE_H__
#define __MYGUI_I_CROPPED_RECTANGLE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	class MYGUI_EXPORT ICroppedRectangle
	{
	public:
		ICroppedRectangle() :
			mIsMargin(false),
			mCroppedParent(nullptr)
		{ }

		virtual ~ICroppedRectangle() { }

		/** Get parent ICroppedRectangle */
		ICroppedRectangle* getCroppedParent()
		{
			return mCroppedParent;
		}

		/** Set position */
		virtual void setPosition(const IntPoint& _value)
		{
			mCoord.left = _value.left;
			mCoord.top = _value.top;
		}
		/** Set size */
		virtual void setSize(const IntSize& _value)
		{
			mCoord.width = _value.width;
			mCoord.height = _value.height;
		}
		/** Set coordinates (position and size) */
		virtual void setCoord(const IntCoord& _value)
		{
			mCoord = _value;
		}

		/** Get position */
		IntPoint getPosition() const
		{
			return mCoord.point();
		}
		/** Get size */
		IntSize getSize() const
		{
			return mCoord.size();
		}
		/** Get coordinates (position and size) */
		const IntCoord& getCoord() const
		{
			return mCoord;
		}

		/** Get position in screen coordinates */
		const IntPoint& getAbsolutePosition() const
		{
			return mAbsolutePosition;
		}
		/** Get rectangle in screen coordinates */
		IntRect getAbsoluteRect() const
		{
			return IntRect(mAbsolutePosition.left, mAbsolutePosition.top, mAbsolutePosition.left + mCoord.width, mAbsolutePosition.top + mCoord.height);
		}
		/** Get coordinate in screen coordinates */
		IntCoord getAbsoluteCoord() const
		{
			return IntCoord(mAbsolutePosition.left, mAbsolutePosition.top, mCoord.width, mCoord.height);
		}

		/** Get X in screen coordinates */
		int getAbsoluteLeft() const
		{
			return mAbsolutePosition.left;
		}
		/** Get Y in screen coordinates */
		int getAbsoluteTop() const
		{
			return mAbsolutePosition.top;
		}

		/** Get left x-coordinate */
		int getLeft() const
		{
			return mCoord.left;
		}
		/** Get right x-coordinate */
		int getRight() const
		{
			return mCoord.right();
		}
		/** Get top y-coordinate */
		int getTop() const
		{
			return mCoord.top;
		}
		/** Get bottom y-coordinate */
		int getBottom() const
		{
			return mCoord.bottom();
		}
		/** Get width */
		int getWidth() const
		{
			return mCoord.width;
		}
		/** Get height */
		int getHeight() const
		{
			return mCoord.height;
		}


		/*internal:*/
		/** True if rectangle is cropped by parent rectangle */
		bool _isMargin() const
		{
			return mIsMargin;
		}

		// Get cropped by parent rectangle coordinates
		int _getViewLeft() const
		{
			return mCoord.left + mMargin.left;
		}
		int _getViewRight() const
		{
			return mCoord.right() - mMargin.right;
		}
		int _getViewTop() const
		{
			return mCoord.top + mMargin.top;
		}
		int _getViewBottom() const
		{
			return mCoord.bottom() - mMargin.bottom;
		}
		int _getViewWidth() const
		{
			return mCoord.width - mMargin.left - mMargin.right;
		}
		int _getViewHeight() const
		{
			return mCoord.height - mMargin.top - mMargin.bottom;
		}

		void _setCroppedParent(ICroppedRectangle* _parent)
		{
			mCroppedParent = _parent;
		}

		const IntRect& _getMargin() const
		{
			return mMargin;
		}
		int _getMarginLeft() const
		{
			return mMargin.left;
		}
		int _getMarginRight() const
		{
			return mMargin.right;
		}
		int _getMarginTop() const
		{
			return mMargin.top;
		}
		int _getMarginBottom() const
		{
			return mMargin.bottom;
		}

	protected:
		bool _checkMargin()
		{
			bool margin = false;
			//?????????????? ???? ????????????
			if (getLeft() < mCroppedParent->mMargin.left)
			{
				mMargin.left = mCroppedParent->mMargin.left - getLeft();
				margin = true;
			}
			else
			{
				mMargin.left = 0;
			}

			//?????????????? ???? ??????????????
			if (getRight() > mCroppedParent->getWidth() - mCroppedParent->mMargin.right)
			{
				mMargin.right = getRight() - (mCroppedParent->getWidth() - mCroppedParent->mMargin.right);
				margin = true;
			}
			else
			{
				mMargin.right = 0;
			}

			//?????????????? ???? ??????????
			if (getTop() < mCroppedParent->mMargin.top)
			{
				mMargin.top = mCroppedParent->mMargin.top - getTop();
				margin = true;
			}
			else
			{
				mMargin.top = 0;
			}

			//?????????????? ???? ????????
			if (getBottom() > mCroppedParent->getHeight() - mCroppedParent->mMargin.bottom)
			{
				mMargin.bottom = getBottom() - (mCroppedParent->getHeight() - mCroppedParent->mMargin.bottom);
				margin = true;
			}
			else
			{
				mMargin.bottom = 0;
			}

			return margin;
		}

		bool _checkOutside() const // ???????????????? ???? ???????????? ?????????? ???? ??????????????
		{
			return ( (getRight() < mCroppedParent->mMargin.left ) || // ???????????? ???????????? ????????????
					(getLeft() > mCroppedParent->getWidth() - mCroppedParent->mMargin.right ) || // ???????????? ???????????? ??????????????
					(getBottom() < mCroppedParent->mMargin.top  ) || // ???????????? ???????????? ??????????
					(getTop() > mCroppedParent->getHeight() - mCroppedParent->mMargin.bottom ) );  // ???????????? ???????????? ????????
		}

	protected:
		IntRect mMargin; // ????????????????????
		IntCoord mCoord; // ????????????????????
		IntPoint mAbsolutePosition; // ???????????????????? ????????????????????

		bool mIsMargin;
		ICroppedRectangle* mCroppedParent;
	};

} // namespace MyGUI

#endif // __MYGUI_I_CROPPED_RECTANGLE_H__
