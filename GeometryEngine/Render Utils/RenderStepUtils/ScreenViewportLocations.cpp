#include "ScreenViewportLocations.h"

GeometryEngine::GeometryRenderStep::ScreenViewportLocation::ScreenViewportLocation(int viewportNumber)
{
	generateViewportSizes(viewportNumber, mViewportSize);
	mErrorValues = QVector4D(-1, -1, -1, -1);
}

GeometryEngine::GeometryRenderStep::ScreenViewportLocation::ScreenViewportLocation(const std::map<GeometryWorldItem::GeometryCamera::CameraTargets, QVector4D>& viewportMap)
{
	fillViewportMap(viewportMap);
	mErrorValues = QVector4D(-1, -1, -1, -1);
}

GeometryEngine::GeometryRenderStep::ScreenViewportLocation::~ScreenViewportLocation()
{
}

const QVector4D & GeometryEngine::GeometryRenderStep::ScreenViewportLocation::GetViewportSize(const GeometryWorldItem::GeometryCamera::CameraTargets & target)
{
	if (ContainsViewport(target)) return mViewportSize[target];
	return mErrorValues;
}

QVector4D GeometryEngine::GeometryRenderStep::ScreenViewportLocation::GetQuadVertexPositions(const QVector4D & screenSize, const QVector4D& viewportSize)
{
	float x = screenSize.x() + (viewportSize.x() * screenSize.z());
	float y = screenSize.y() + (viewportSize.y() * screenSize.w());
	return QVector4D(x, y, x + viewportSize.z() * screenSize.z(), y + viewportSize.w() * screenSize.w());
}

bool GeometryEngine::GeometryRenderStep::ScreenViewportLocation::ContainsViewport(const GeometryWorldItem::GeometryCamera::CameraTargets & target)
{
	return mViewportSize.find(target) != mViewportSize.end();
}

bool GeometryEngine::GeometryRenderStep::ScreenViewportLocation::AddViewport(GeometryWorldItem::GeometryCamera::CameraTargets target, QVector4D viewportSize)
{
	if (!ContainsViewport(target))
	{
		mViewportSize[target] = clampValues(viewportSize);
	}

	return false;
}

bool GeometryEngine::GeometryRenderStep::ScreenViewportLocation::ModifyViewport(GeometryWorldItem::GeometryCamera::CameraTargets target, QVector4D viewportSize)
{
	if (ContainsViewport(target))
	{
		mViewportSize[target] = clampValues(viewportSize);
		return true;
	}

	return false;
}

bool GeometryEngine::GeometryRenderStep::ScreenViewportLocation::RemoveViewport(const GeometryWorldItem::GeometryCamera::CameraTargets & target)
{
	if (ContainsViewport(target))
	{
		mViewportSize.erase(target);
		return true;
	}

	return false;
}

QVector4D& GeometryEngine::GeometryRenderStep::ScreenViewportLocation::clampValues(QVector4D & vector)
{
	vector.setX(vector.x() < 0 ? 0 : (vector.x() > 1 ? 1 : vector.x() ));
	vector.setY(vector.y() < 0 ? 0 : (vector.y() > 1 ? 1 : vector.y() ));
	vector.setW(vector.w() < 0 ? 0 : (vector.w() > 1 ? 1 : vector.w() ));
	vector.setZ(vector.z() < 0 ? 0 : (vector.z() > 1 ? 1 : vector.z() ));
	return vector;
}

void GeometryEngine::GeometryRenderStep::ScreenViewportLocation::fillViewportMap(const std::map<GeometryWorldItem::GeometryCamera::CameraTargets, QVector4D>& viewportMap)
{
	mViewportSize.clear();

	for (auto iter = viewportMap.begin(); iter != viewportMap.end(); ++iter)
	{
		mViewportSize[(*iter).first] = (*iter).second;
	}
}

void GeometryEngine::GeometryRenderStep::ScreenViewportLocation::generateViewportSizes(int viewportNumber, std::map<GeometryWorldItem::GeometryCamera::CameraTargets, QVector4D>& viewportMap)
{
	viewportMap.clear();

	int numRows = std::sqrt(viewportNumber); int numCols = numRows;
	if (viewportNumber > 1 && numRows < 2) numRows = 2;

	int extraViewports = viewportNumber - (numRows * numCols);

	int counter = 0;
	GeometryWorldItem::GeometryCamera::CameraTargets target = GeometryWorldItem::GeometryCamera::CameraTargets::CAM1;

	for (int i = 0; i < numRows; ++i)
	{
		int modNumCols = extraViewports > 0 ? numCols + 1 : numCols;

		for (int j = 0; j < modNumCols; ++j)
		{
			QVector4D viewportSize(((float)j / (float)modNumCols), ((float)i / (float)numRows), (1.0 / (float)modNumCols), (1.0 / (float)numRows));
			viewportMap[(GeometryWorldItem::GeometryCamera::CameraTargets)(target + counter++)] = viewportSize;
		}

		--extraViewports;
	}
}

void GeometryEngine::GeometryRenderStep::ScreenViewportLocation::copy(const ScreenViewportLocation & ref)
{
	fillViewportMap(ref.mViewportSize);
	mErrorValues = ref.mErrorValues;
}
