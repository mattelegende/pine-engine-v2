#pragma once
#include "../IComponent/IComponent.hpp"
#include <glm/vec3.hpp>

namespace Pine {

	enum ELightType {
		Directional,
		PointLight
	};

	class Light : public IComponent {
	private:
		glm::vec3 m_LightColor = glm::vec3( 1.f, 1.f, 1.f );
		ELightType m_LightType = ELightType::Directional;

		glm::vec3 m_Attenuation = glm::vec3( 1.f, 0.045f, 0.0075 );
	public:
		Light( );

		void SetLightColor( glm::vec3 lightColor );
		const glm::vec3& GetLightColor( ) const;

		void SetLightType( ELightType type );
		ELightType GetLightType( ) const;

		void SetAttenuation( glm::vec3 vec );
		const glm::vec3& GetAttenuation( ) const;
		
		void OnSetup( ) override;
		void OnUpdate( float deltaTime ) override;

		void SaveToJson( nlohmann::json& j ) override;
		void LoadFromJson( nlohmann::json& j ) override;
		IComponent* Clone( ) override;
	};

}