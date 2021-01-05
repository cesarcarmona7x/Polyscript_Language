#include "stdafx.h"
float PixelToGLPoint2(float pixel,float dimensionSize){
	if(pixel<dimensionSize/2.f){
		return (pixel/(dimensionSize/2.f))-1.f;
	}
	else if(pixel==dimensionSize/2.f){
		return 0.f;
	}
	else{
		return (pixel-(dimensionSize/2.f))/(dimensionSize-(dimensionSize/2.f));
	}
}
const char* getErrorMessage2(FT_Error err)
{
#undef FTERRORS_H_
#define FT_ERRORDEF( e, v, s )  case e: return s;
#define FT_ERROR_START_LIST     switch (err) {
#define FT_ERROR_END_LIST       }
#include FT_ERRORS_H
	return "(Unknown error)";
}
PolyscriptFigure::PolyscriptFigure(PolyscriptFigureDesc desc){
	this->desc=desc;
}
void PolyscriptFigure::recreateResources(std::shared_ptr<D2DHandle>& d2d){
	bool useBrush=desc.brush!=PolyscriptBrush();
	bool useGradient=desc.gradient!=PolyscriptGradient();
	bool useBitmap=desc.bitmap!=PolyscriptBitmap();
	switch(desc.type){
	case POLYSCRIPT_FIGURE_TYPE_TRIANGLE:
		d2d->d2dfactory->CreatePathGeometry(polygon.ReleaseAndGetAddressOf());
		polygon->Open(polygonsink.GetAddressOf());
		polygonsink->SetFillMode(D2D1_FILL_MODE_WINDING);
		polygonsink->BeginFigure(Point2F(desc.points.at(0).x,desc.points.at(0).y),D2D1_FIGURE_BEGIN_FILLED);
		for(int i=1;i<3;i++){
			polygonsink->AddLine(Point2F(desc.points.at(i).x,desc.points.at(i).y));
		}
		polygonsink->EndFigure(D2D1_FIGURE_END_CLOSED);
		break;
	case POLYSCRIPT_FIGURE_TYPE_POLYGON:
		d2d->d2dfactory->CreatePathGeometry(polygon.ReleaseAndGetAddressOf());
		polygon->Open(polygonsink.GetAddressOf());
		polygonsink->SetFillMode(D2D1_FILL_MODE_WINDING);
		polygonsink->BeginFigure(Point2F(desc.points.at(0).x,desc.points.at(0).y),D2D1_FIGURE_BEGIN_FILLED);
		for(int i=1;i<desc.points.size();i++){
			polygonsink->AddLine(Point2F(desc.points.at(i).x,desc.points.at(i).y));
		}
		polygonsink->EndFigure(D2D1_FIGURE_END_CLOSED);
		polygonsink->Close();
		break;
	}
	if(useBrush){
		d2d->target->CreateSolidColorBrush(ColorF(desc.color.getDataColor()[0],desc.color.getDataColor()[1],desc.color.getDataColor()[2],desc.color.getDataColor()[3]*desc.opacity),solidbrush1.ReleaseAndGetAddressOf());
	}
	else if(useGradient){

	}
	else if(useBitmap){
	}
}
void PolyscriptFigure::recreateResources(std::shared_ptr<GLHandle>& gl){
	
}
void PolyscriptFigure::draw(std::shared_ptr<D2DHandle>& d2d){
	bool useBrush=solidbrush1!=nullptr;
	bool useGradient=gradientbrush1!=nullptr;
	bool useBitmap=bmpbrush1!=nullptr;
	switch(desc.type){
	case POLYSCRIPT_FIGURE_TYPE_SQUARE:
		switch(desc.drawMode){
		case POLYSCRIPT_DRAW_MODE_DRAW:
			if(useBrush){
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),solidbrush1.Get());
			}
			else if(useGradient){
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),gradientbrush1.Get());
			}
			else if(useBitmap){
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),bmpbrush1.Get());
			}
			break;
		case POLYSCRIPT_DRAW_MODE_FILL:
			if(useBrush){
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),solidbrush1.Get());
			}
			else if(useGradient){
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),gradientbrush1.Get());
			}
			else if(useBitmap){
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),bmpbrush1.Get());
			}
			break;
		case POLYSCRIPT_DRAW_MODE_BOTH_DRAW_TOP:
			if(useBrush){
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),solidbrush1.Get());
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),solidbrush2.Get());
			}
			else if(useGradient){
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),gradientbrush1.Get());
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),gradientbrush2.Get());
			}
			else if(useBitmap){
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),bmpbrush1.Get());
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),bmpbrush2.Get());
			}
			break;
		case POLYSCRIPT_DRAW_MODE_BOTH_FILL_TOP:
			if(useBrush){
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),solidbrush1.Get());
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),solidbrush2.Get());
			}
			else if(useGradient){
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),gradientbrush1.Get());
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),gradientbrush2.Get());
			}
			else if(useBitmap){
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),bmpbrush1.Get());
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.width)?desc.y+desc.width:(desc.top&&desc.width)?desc.top+desc.width:desc.bottom),bmpbrush2.Get());
			}
			break;
		}
		break;
	case POLYSCRIPT_FIGURE_TYPE_RECTANGLE:
		switch(desc.drawMode){
		case POLYSCRIPT_DRAW_MODE_DRAW:
			if(useBrush){
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),solidbrush1.Get());
			}
			else if(useGradient){
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),gradientbrush1.Get());
			}
			else if(useBitmap){
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),bmpbrush1.Get());
			}
			break;
		case POLYSCRIPT_DRAW_MODE_FILL:
			if(useBrush){
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),solidbrush1.Get());
			}
			else if(useGradient){
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),gradientbrush1.Get());
			}
			else if(useBitmap){
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),bmpbrush1.Get());
			}
			break;
		case POLYSCRIPT_DRAW_MODE_BOTH_DRAW_TOP:
			if(useBrush){
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),solidbrush1.Get());
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),solidbrush2.Get());
			}
			else if(useGradient){
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),gradientbrush1.Get());
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),gradientbrush2.Get());
			}
			else if(useBitmap){
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),bmpbrush1.Get());
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),bmpbrush2.Get());
			}
			break;
		case POLYSCRIPT_DRAW_MODE_BOTH_FILL_TOP:
			if(useBrush){
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),solidbrush1.Get());
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),solidbrush2.Get());
			}
			else if(useGradient){
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),gradientbrush1.Get());
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),gradientbrush2.Get());
			}
			else if(useBitmap){
				d2d->target->DrawRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),bmpbrush1.Get());
				d2d->target->FillRectangle(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),bmpbrush2.Get());
			}
			break;
		}
		break;
	case POLYSCRIPT_FIGURE_TYPE_ROUNDEDRECTANGLE:
		switch(desc.drawMode){
		case POLYSCRIPT_DRAW_MODE_DRAW:
			if(useBrush){
				d2d->target->DrawRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),solidbrush1.Get());
			}
			else if(useGradient){
				d2d->target->DrawRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),gradientbrush1.Get());
			}
			else if(useBitmap){
				d2d->target->DrawRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),bmpbrush1.Get());
			}
			break;
		case POLYSCRIPT_DRAW_MODE_FILL:
			if(useBrush){
				d2d->target->FillRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),solidbrush1.Get());
			}
			else if(useGradient){
				d2d->target->FillRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),gradientbrush1.Get());
			}
			else if(useBitmap){
				d2d->target->FillRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),bmpbrush1.Get());
			}
			break;
		case POLYSCRIPT_DRAW_MODE_BOTH_DRAW_TOP:
			if(useBrush){
				d2d->target->FillRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),solidbrush1.Get());
				d2d->target->DrawRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),solidbrush2.Get());
			}
			else if(useGradient){
				d2d->target->FillRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),gradientbrush1.Get());
				d2d->target->DrawRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),gradientbrush2.Get());
			}
			else if(useBitmap){
				d2d->target->FillRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),bmpbrush1.Get());
				d2d->target->DrawRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),bmpbrush2.Get());
			}
			break;
		case POLYSCRIPT_DRAW_MODE_BOTH_FILL_TOP:
			if(useBrush){
				d2d->target->DrawRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),solidbrush1.Get());
				d2d->target->FillRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),solidbrush2.Get());
			}
			else if(useGradient){
				d2d->target->DrawRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),gradientbrush1.Get());
				d2d->target->FillRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),gradientbrush2.Get());
			}
			else if(useBitmap){
				d2d->target->DrawRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),bmpbrush1.Get());
				d2d->target->FillRoundedRectangle(RoundedRect(RectF((desc.x)?desc.x:desc.left,(desc.y)?desc.y:desc.top,(desc.x&&desc.width)?desc.x+desc.width:(desc.left&&desc.width)?desc.left+desc.width:desc.right,(desc.y&&desc.height)?desc.y+desc.height:(desc.top&&desc.height)?desc.top+desc.height:desc.bottom),desc.radiusX,desc.radiusY),bmpbrush2.Get());
			}
			break;
		}
		break;
	case POLYSCRIPT_FIGURE_TYPE_ELLIPSE:
		switch(desc.drawMode){
		case POLYSCRIPT_DRAW_MODE_DRAW:
			if(useBrush){
				d2d->target->DrawEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),solidbrush1.Get());
			}
			else if(useGradient){
				d2d->target->DrawEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),gradientbrush1.Get());
			}
			else if(useBitmap){
				d2d->target->DrawEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),bmpbrush1.Get());
			}
			break;
		case POLYSCRIPT_DRAW_MODE_FILL:
			if(useBrush){
				d2d->target->FillEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),solidbrush1.Get());
			}
			else if(useGradient){
				d2d->target->FillEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),gradientbrush1.Get());
			}
			else if(useBitmap){
				d2d->target->FillEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),bmpbrush1.Get());
			}
			break;
		case POLYSCRIPT_DRAW_MODE_BOTH_DRAW_TOP:
			if(useBrush){
				d2d->target->FillEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),solidbrush1.Get());
				d2d->target->DrawEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),solidbrush2.Get());
			}
			else if(useGradient){
				d2d->target->FillEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),gradientbrush1.Get());
				d2d->target->DrawEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),gradientbrush2.Get());
			}
			else if(useBitmap){
				d2d->target->FillEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),bmpbrush1.Get());
				d2d->target->DrawEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),bmpbrush2.Get());
			}
			break;
		case POLYSCRIPT_DRAW_MODE_BOTH_FILL_TOP:
			if(useBrush){
				d2d->target->DrawEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),solidbrush1.Get());
				d2d->target->FillEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),solidbrush2.Get());
			}
			else if(useGradient){
				d2d->target->DrawEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),gradientbrush1.Get());
				d2d->target->FillEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),gradientbrush2.Get());
			}
			else if(useBitmap){
				d2d->target->DrawEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),bmpbrush1.Get());
				d2d->target->FillEllipse(Ellipse(Point2F(desc.x+desc.radiusX,desc.y+desc.radiusY),desc.radiusX,desc.radiusY),bmpbrush2.Get());
			}
			break;
		}
		break;
	case POLYSCRIPT_FIGURE_TYPE_LINE:
		if(useBrush){
			d2d->target->DrawLine(Point2F(desc.points.at(0).x,desc.points.at(0).y),Point2F(desc.points.at(1).x,desc.points.at(1).y),solidbrush1.Get());
		}
		else if(useGradient){
			d2d->target->DrawLine(Point2F(desc.points.at(0).x,desc.points.at(0).y),Point2F(desc.points.at(1).x,desc.points.at(1).y),gradientbrush1.Get());
		}
		else if(useBitmap){
			d2d->target->DrawLine(Point2F(desc.points.at(0).x,desc.points.at(0).y),Point2F(desc.points.at(1).x,desc.points.at(1).y),bmpbrush1.Get());
		}
		break;
	case POLYSCRIPT_FIGURE_TYPE_TRIANGLE:
	case POLYSCRIPT_FIGURE_TYPE_POLYGON:
		switch(desc.drawMode){
		case POLYSCRIPT_DRAW_MODE_DRAW:
			if(useBrush){
				d2d->target->DrawGeometry(polygon.Get(),solidbrush1.Get());
			}
			else if(useGradient){
				d2d->target->DrawGeometry(polygon.Get(),gradientbrush1.Get());
			}
			else if(useBitmap){
				d2d->target->DrawGeometry(polygon.Get(),bmpbrush1.Get());
			}
			break;
		case POLYSCRIPT_DRAW_MODE_FILL:
			if(useBrush){
				d2d->target->FillGeometry(polygon.Get(),solidbrush1.Get());
			}
			else if(useGradient){
				d2d->target->FillGeometry(polygon.Get(),gradientbrush1.Get());
			}
			else if(useBitmap){
				d2d->target->FillGeometry(polygon.Get(),bmpbrush1.Get());
			}
			break;
		case POLYSCRIPT_DRAW_MODE_BOTH_DRAW_TOP:
			if(useBrush){
				d2d->target->FillGeometry(polygon.Get(),solidbrush1.Get());
				d2d->target->DrawGeometry(polygon.Get(),solidbrush2.Get());
			}
			else if(useGradient){
				d2d->target->FillGeometry(polygon.Get(),gradientbrush1.Get());
				d2d->target->DrawGeometry(polygon.Get(),gradientbrush2.Get());
			}
			else if(useBitmap){
				d2d->target->FillGeometry(polygon.Get(),bmpbrush1.Get());
				d2d->target->DrawGeometry(polygon.Get(),bmpbrush2.Get());
			}
			break;
		case POLYSCRIPT_DRAW_MODE_BOTH_FILL_TOP:
			if(useBrush){
				d2d->target->DrawGeometry(polygon.Get(),solidbrush1.Get());
				d2d->target->FillGeometry(polygon.Get(),solidbrush2.Get());
			}
			else if(useGradient){
				d2d->target->DrawGeometry(polygon.Get(),gradientbrush1.Get());
				d2d->target->FillGeometry(polygon.Get(),gradientbrush2.Get());
			}
			else if(useBitmap){
				d2d->target->DrawGeometry(polygon.Get(),bmpbrush1.Get());
				d2d->target->FillGeometry(polygon.Get(),bmpbrush2.Get());
			}
			break;
		}
		break;
	}
}
void DrawEllipse(float cx, float cy, float rx, float ry, int num_segments) 
{ 
    float theta = 2 * 3.1415926 / float(num_segments); 
    float c = cosf(theta);//precalculate the sine and cosine
    float s = sinf(theta);
    float t;

    float x = 1;//we start at angle = 0 
    float y = 0; 

	glBegin(GL_LINE_LOOP);
    for(int ii = 0; ii < num_segments; ii++) 
    { 
        //apply radius and offset
        glVertex2f(x * rx + cx, y * ry + cy);//output vertex 

        //apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    } 
    glEnd(); 
}
void FillEllipse(float cx, float cy, float rx, float ry, int num_segments) 
{ 
    float theta = 2 * 3.1415926 / float(num_segments); //360°
    float c = cosf(theta);//precalculate the sine and cosine
    float s = sinf(theta);
    float t;

    float x = 1;//we start at angle = 0 
    float y = 0; 

	glBegin(GL_TRIANGLE_FAN);
    for(int ii = 0; ii < num_segments; ii++) 
    { 
        //apply radius and offset
        glVertex2f(x * rx + cx, y * ry + cy);//output vertex 

        //apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    } 
    glEnd(); 
}
void DrawRoundedRectangle(float x1,float y1,float x2,float y2,float rx,float ry,float num_segments){
	float theta = 2 * 3.1415926 / float(num_segments); //360°
	float c=cosf(theta);
	float s=sinf(theta);
	float t;
	glRectf(x1+rx,y1+ry,x2-rx,y2-ry);//Center
	glRectf(x1+rx,y1,x2-rx,y1+ry);//Top
	glRectf(x1,y1+ry,x1+rx,y2-ry);//Left
	glRectf(x2-rx,y1+ry,x2,y2-ry);//Right
	glRectf(x1+rx,y2-ry,x2-rx,y2);//Bottom
	//Top-left corner
	float tlcX=x1+rx;
	float tlcY=y1+ry;
	float x=1;
	float y=0;
	glBegin(GL_TRIANGLE_FAN);
	for(int ii=0;ii<num_segments;ii++){
		glVertex2f(-(x*rx)+tlcX,-(y*ry)+tlcY);
		t=x;
		x=c*x-s*y;
		y=s*t+c*y;
	}
	glEnd();
	//Bottom-left corner
	float blcX=x1+rx;
	float blcY=y2-ry;
	x=1;
	y=0;
	glBegin(GL_TRIANGLE_FAN);
	for(int ii=0;ii<num_segments;ii++){
		glVertex2f(-(x*rx)+blcX,+(y*ry)+blcY);
		t=x;
		x=c*x-s*y;
		y=s*t+c*y;
	}
	glEnd();
	//Top-right corner
	float trcX=x2-rx;
	float trcY=y1+ry;
	x=1;
	y=0;
	glBegin(GL_TRIANGLE_FAN);
	for(int ii=0;ii<num_segments;ii++){
		glVertex2f(+(x*rx)+trcX,-(y*ry)+trcY);
		t=x;
		x=c*x-s*y;
		y=s*t+c*y;
	}
	glEnd();
	//Bottom-right corner
	float brcX=x2-rx;
	float brcY=y2-ry;
	x=1;
	y=0;
	glBegin(GL_TRIANGLE_FAN);
	for(int ii=0;ii<num_segments;ii++){
		glVertex2f(+(x*rx)+brcX,+(y*ry)+brcY);
		t=x;
		x=c*x-s*y;
		y=s*t+c*y;
	}
	glEnd();
}
void PolyscriptFigure::draw(std::shared_ptr<GLHandle>& gl){
	switch(desc.type){
	case POLYSCRIPT_FIGURE_TYPE_SQUARE:
		glPushMatrix();
		switch(desc.drawMode){
		case POLYSCRIPT_DRAW_MODE_DRAW:
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glColor4f(desc.color.getDataColor()[0],desc.color.getDataColor()[1],desc.color.getDataColor()[2],desc.color.getDataColor()[3]);
			glRectf((desc.x)?PixelToGLPoint2(desc.x,1280.f):PixelToGLPoint2(desc.left,1280.f),(desc.y)?PixelToGLPoint2(desc.y,720.f):PixelToGLPoint2(desc.top,720.f),(desc.x&&desc.width)?PixelToGLPoint2(desc.x+desc.width,1280.f):(desc.left&&desc.width)?PixelToGLPoint2(desc.left+desc.width,1280.f):PixelToGLPoint2(desc.right,1280.f),(desc.y&&desc.width)?PixelToGLPoint2(desc.y+desc.width,720.f):(desc.top&&desc.width)?PixelToGLPoint2(desc.top+desc.width,720.f):PixelToGLPoint2(desc.bottom,720.f));
			break;
		case POLYSCRIPT_DRAW_MODE_FILL:
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			glColor4f(desc.color.getDataColor()[0],desc.color.getDataColor()[1],desc.color.getDataColor()[2],desc.color.getDataColor()[3]);
			glRectf((desc.x)?PixelToGLPoint2(desc.x,1280.f):PixelToGLPoint2(desc.left,1280.f),(desc.y)?PixelToGLPoint2(desc.y,720.f):PixelToGLPoint2(desc.top,720.f),(desc.x&&desc.width)?PixelToGLPoint2(desc.x+desc.width,1280.f):(desc.left&&desc.width)?PixelToGLPoint2(desc.left+desc.width,1280.f):PixelToGLPoint2(desc.right,1280.f),(desc.y&&desc.width)?PixelToGLPoint2(desc.y+desc.width,720.f):(desc.top&&desc.width)?PixelToGLPoint2(desc.top+desc.width,720.f	):PixelToGLPoint2(desc.bottom,720.f));
			break;
		}
		glPopMatrix();
		break;
	case POLYSCRIPT_FIGURE_TYPE_RECTANGLE:
		glPushMatrix();
		switch(desc.drawMode){
		case POLYSCRIPT_DRAW_MODE_DRAW:
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glColor4f(desc.color.getDataColor()[0],desc.color.getDataColor()[1],desc.color.getDataColor()[2],desc.color.getDataColor()[3]);
			glRectf((desc.x)?PixelToGLPoint2(desc.x,1280.f):PixelToGLPoint2(desc.left,1280.f),(desc.y)?PixelToGLPoint2(desc.y,720.f):PixelToGLPoint2(desc.top,720.f),(desc.x&&desc.width)?PixelToGLPoint2(desc.x+desc.width,1280.f):(desc.left&&desc.width)?PixelToGLPoint2(desc.left+desc.width,1280.f):PixelToGLPoint2(desc.right,1280.f),(desc.y&&desc.height)?PixelToGLPoint2(desc.y+desc.height,720.f):(desc.top&&desc.height)?PixelToGLPoint2(desc.top+desc.height,720.f):PixelToGLPoint2(desc.bottom,720.f));
			break;
		case POLYSCRIPT_DRAW_MODE_FILL:
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			glColor4f(desc.color.getDataColor()[0],desc.color.getDataColor()[1],desc.color.getDataColor()[2],desc.color.getDataColor()[3]);
			glRectf((desc.x)?PixelToGLPoint2(desc.x,1280.f):PixelToGLPoint2(desc.left,1280.f),(desc.y)?PixelToGLPoint2(desc.y,720.f):PixelToGLPoint2(desc.top,720.f),(desc.x&&desc.width)?PixelToGLPoint2(desc.x+desc.width,1280.f):(desc.left&&desc.width)?PixelToGLPoint2(desc.left+desc.width,1280.f):PixelToGLPoint2(desc.right,1280.f),(desc.y&&desc.height)?PixelToGLPoint2(desc.y+desc.height,720.f):(desc.top&&desc.height)?PixelToGLPoint2(desc.top+desc.height,720.f):PixelToGLPoint2(desc.bottom,720.f));
			break;
		}
		glPopMatrix();
		break;
	case POLYSCRIPT_FIGURE_TYPE_ROUNDEDRECTANGLE:
		glPushMatrix();
		switch(desc.drawMode){
		case POLYSCRIPT_DRAW_MODE_DRAW:
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glColor4f(desc.color.getDataColor()[0],desc.color.getDataColor()[1],desc.color.getDataColor()[2],desc.color.getDataColor()[3]);
			DrawRoundedRectangle((desc.x)?PixelToGLPoint2(desc.x,1280.f):PixelToGLPoint2(desc.left,1280.f),(desc.y)?PixelToGLPoint2(desc.y,720.f):PixelToGLPoint2(desc.top,720.f),(desc.x&&desc.width)?PixelToGLPoint2(desc.x+desc.width,1280.f):(desc.left&&desc.width)?PixelToGLPoint2(desc.left+desc.width,1280.f):PixelToGLPoint2(desc.right,1280.f),(desc.y&&desc.height)?PixelToGLPoint2(desc.y+desc.height,720.f):(desc.top&&desc.height)?PixelToGLPoint2(desc.top+desc.height,720.f):PixelToGLPoint2(desc.bottom,720.f),(desc.radiusX/1280.f)*2,(desc.radiusY/720.f)*2,128);
			break;
		case POLYSCRIPT_DRAW_MODE_FILL:
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			glColor4f(desc.color.getDataColor()[0],desc.color.getDataColor()[1],desc.color.getDataColor()[2],desc.color.getDataColor()[3]);
			DrawRoundedRectangle((desc.x)?PixelToGLPoint2(desc.x,1280.f):PixelToGLPoint2(desc.left,1280.f),(desc.y)?PixelToGLPoint2(desc.y,720.f):PixelToGLPoint2(desc.top,720.f),(desc.x&&desc.width)?PixelToGLPoint2(desc.x+desc.width,1280.f):(desc.left&&desc.width)?PixelToGLPoint2(desc.left+desc.width,1280.f):PixelToGLPoint2(desc.right,1280.f),(desc.y&&desc.height)?PixelToGLPoint2(desc.y+desc.height,720.f):(desc.top&&desc.height)?PixelToGLPoint2(desc.top+desc.height,720.f):PixelToGLPoint2(desc.bottom,720.f),(desc.radiusX/1280.f)*2,(desc.radiusY/720.f)*2,128);
			break;
		}
		glPopMatrix();
		break;
	case POLYSCRIPT_FIGURE_TYPE_ELLIPSE:
		glPushMatrix();
		switch(desc.drawMode){
		case POLYSCRIPT_DRAW_MODE_DRAW:
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glColor4f(desc.color.getDataColor()[0],desc.color.getDataColor()[1],desc.color.getDataColor()[2],desc.color.getDataColor()[3]);
			DrawEllipse(PixelToGLPoint2(desc.x+desc.radiusX,1280.f),PixelToGLPoint2(desc.y+desc.radiusY,720.f),(desc.radiusX/1280.f)*2,(desc.radiusY/720.f)*2,128);
			break;
		case POLYSCRIPT_DRAW_MODE_FILL:
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			glColor4f(desc.color.getDataColor()[0],desc.color.getDataColor()[1],desc.color.getDataColor()[2],desc.color.getDataColor()[3]);
			FillEllipse(PixelToGLPoint2(desc.x+desc.radiusX,1280.f),PixelToGLPoint2(desc.y+desc.radiusY,720.f),(desc.radiusX/1280.f)*2,(desc.radiusY/720.f)*2,128);
			break;
		}
		glPopMatrix();
		break;
	}
}