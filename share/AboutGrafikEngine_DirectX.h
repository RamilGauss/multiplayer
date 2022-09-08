/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef AboutGrafikEngine_DirectXH
#define AboutGrafikEngine_DirectXH


/*
1) Террейн(земля)?изменение земли под действием снарядов или самих танков?, - СЕРВЕР
2) Объекты:
   по типу "разрушаем" и по типу "подвижность"
	 2.1. Разрушаемые и неподвижные (дом, стул и т.д.), - СЕРВЕР
	 2.2. Разрушаемые и подвижные (танк), - СЕРВЕР
	 2.3. Не разрушаемые и неподвижные (дом), - КЛИЕНТ
	 2.4. Не разрушаемые и подвижные (), - СЕРВЕР
3) skybox(небо), - КЛИЕНТ
4) Вода, имеет уровень, возможно имеет несколько уровней или даже объем. - КЛИЕНТ


Все пространство делится на чанки. Вообще и для движка на сервере почти то же самое.

Каждый объект имеет BoundingBox. Куб в который входит объект. Для деревьев и других динамических объектов (не разрушаемых) куб вращается в пространстве при изменении состояния, например при
падении дерева или столба.
У каждого объекта есть состояния.



*/

#endif