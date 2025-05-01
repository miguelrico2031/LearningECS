#include "BaseEditor.h"

namespace ObjectComponent
{
	class Editor : public BaseEditor
	{
	public:
		void onAttach() override;
		void update() override;
		void fixedUpdate() override;
	};
}