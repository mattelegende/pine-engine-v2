#include "Widgets.hpp"
#include <imgui/imgui.h>
#include <ImGui/imgui_internal.h>
#include "Pine/Assets/Texture2D/Texture2D.hpp"
#include <optional>
#include <windows.h>
#include <Pine/Core/Window/Window.hpp>
#include <Pine/Assets/Assets.hpp>
#include <Pine/Assets/IAsset/IAsset.hpp>
#include <filesystem>
#include "../Gui.hpp"


namespace {
	std::optional<std::string> OpenFile( const char* filter ) {
		OPENFILENAMEA ofn;
		CHAR szFile[ 260 ] = { 0 };
		ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
		ofn.lStructSize = sizeof( OPENFILENAME );
		ofn.hwndOwner = reinterpret_cast< HWND >( Pine::Window::Internal::GetWindowHWND( ) );
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof( szFile );
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if ( GetOpenFileNameA( &ofn ) == TRUE )
			return ofn.lpstrFile;

		return std::nullopt;
	}

	Pine::IAsset* FindAssetFromAbsoulePath( const std::string& path ) {
		for ( auto& a : Pine::Assets::GetAssets( ) ) {
			if ( a.second->GetAbsoluteFilePath( ) == path ) {
				return a.second;
			}
		}

		return nullptr;
	}
}

void Editor::Gui::Widgets::Vector3( const std::string& str, glm::vec3& vec ) {

	const float size = 50.f;

	ImGui::Columns( 2, nullptr, false );

	ImGui::Text( "%s", str.c_str( ) );

	ImGui::NextColumn( );

	ImGui::BeginChild( str.c_str( ), ImVec2( -1.f, 30.f ), false, 0 );

	ImGui::Columns( 3, nullptr, false );

	ImGui::SetNextItemWidth( size );
	ImGui::DragFloat( std::string( "X##" + str ).c_str( ), &vec.x, 0.01f, -FLT_MAX, FLT_MAX );

	ImGui::NextColumn( );

	ImGui::SetNextItemWidth( size );
	ImGui::DragFloat( std::string( "Y##" + str ).c_str( ), &vec.y, 0.01f, -FLT_MAX, FLT_MAX );

	ImGui::NextColumn( );

	ImGui::SetNextItemWidth( size );
	ImGui::DragFloat( std::string( "Z##" + str ).c_str( ), &vec.z, 0.01f, -FLT_MAX, FLT_MAX );

	ImGui::Columns( 1 );

	ImGui::EndChild( );

	ImGui::Columns( 1 );

}

bool Editor::Gui::Widgets::SliderFloat(const std::string& str, float& value, float min, float max, const std::string& format)
{
	ImGui::Columns(2, nullptr, false);

	ImGui::Text("%s", str.c_str());

	ImGui::NextColumn();

	ImGui::SetNextItemWidth(-1.f);

	const bool retValue = ImGui::SliderFloat(std::string("##" + str).c_str(), &value, min, max, format.c_str());
	
	ImGui::Columns(1);

	return retValue;
}

bool Editor::Gui::Widgets::Combobox( const std::string& str, int& value, const char* items )
{
	ImGui::Columns( 2, nullptr, false );

	ImGui::Text( "%s", str.c_str( ) );

	ImGui::NextColumn( );

	ImGui::SetNextItemWidth( -1.f );

	const bool retValue = ImGui::Combo( std::string( "##" + str ).c_str( ), &value, items );

	ImGui::Columns( 1 );

	return retValue;
}

void Editor::Gui::Widgets::PushDisabled( ) {

	ImGui::PushItemFlag( ImGuiItemFlags_Disabled, true );
	ImGui::PushStyleVar( ImGuiStyleVar_Alpha, ImGui::GetStyle( ).Alpha * 0.3f );

}

void Editor::Gui::Widgets::PopDisabled( ) {

	ImGui::PopItemFlag( );
	ImGui::PopStyleVar( );

}

Pine::IAsset* Editor::Gui::Widgets::AssetPicker( const std::string& str, Pine::IAsset* currentAsset /*= nullptr*/, bool shouldRestrictType /*= false*/, Pine::EAssetType type /*= Pine::EAssetType::Invalid */ ) {
	Pine::IAsset* returnValue = nullptr;

	ImGui::Columns( 2, nullptr, false );

	ImGui::Text( "%s", str.c_str( ) );

	ImGui::NextColumn( );

	char buff[ 64 ];

	if ( currentAsset != nullptr ) {
		strcpy_s( buff, currentAsset->GetPath( ).string( ).c_str( ) );
	}
	else {
		strcpy_s( buff, "\0" );
	}

	ImGui::PushStyleVar( ImGuiStyleVar_::ImGuiStyleVar_ItemSpacing, ImVec2( 3.f, 4.f ) );

	ImGui::SetNextItemWidth( ImGui::GetContentRegionAvail( ).x - 60.f );
	ImGui::InputText( std::string( "##" + str ).c_str( ), buff, 64, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly );

	if ( ImGui::BeginDragDropTarget( ) ) {
		if ( auto payload = ImGui::AcceptDragDropPayload( "Asset", 0 ) ) {
			auto asset = *reinterpret_cast< Pine::IAsset** >( payload->Data );

			if ( !shouldRestrictType || ( shouldRestrictType && asset->GetType( ) == type ) ) {
				returnValue = asset;
			}
		}

		ImGui::EndDragDropTarget( );
	}

	if ( ImGui::IsItemHovered( ) && currentAsset != nullptr && currentAsset->GetType( ) == Pine::EAssetType::Texture2D ) {
		auto texture = dynamic_cast< Pine::Texture2D* >( currentAsset );

		ImGui::BeginTooltip( );

		ImGui::Image( reinterpret_cast< ImTextureID >( texture->GetId( ) ), ImVec2( 64.f, 64.f ) );

		ImGui::EndTooltip( );
	}

	if ( ImGui::IsItemClicked( ) && currentAsset != nullptr ) {
		Editor::Gui::Globals::SelectedAssetPtrs.clear( );
		Editor::Gui::Globals::SelectedEntityPtrs.clear( );
		Editor::Gui::Globals::SelectedAssetPtrs.push_back( currentAsset );
	}

	ImGui::SameLine( );

	if ( ImGui::Button( std::string( "...##" + str ).c_str( ) ) ) {
		auto file = OpenFile( "Asset File (*.*)\0*.*\0" );
		if ( file ) {
			if ( auto a = FindAssetFromAbsoulePath( *file ) ) {
				if ( shouldRestrictType && a->GetType( ) == type ) {
					returnValue = a;
				}
			}
			else {
				MessageBoxA( reinterpret_cast< HWND >( Pine::Window::Internal::GetWindowHWND( ) ), "Asset is not loaded.", 0, 0 );
			}
		}
	}

	if ( ImGui::IsItemHovered( ) )
		ImGui::SetTooltip( "Browse Asset" );

	ImGui::SameLine( );

	if ( currentAsset == nullptr )
		PushDisabled( );

	if ( ImGui::Button( std::string( "X##" + str ).c_str( ) ) ) {

	}

	if ( ImGui::IsItemHovered( ) )
		ImGui::SetTooltip( "Remove Asset" );

	if ( currentAsset == nullptr )
		PopDisabled( );

	ImGui::PopStyleVar( );

	ImGui::Columns( 1 );

	return returnValue;
}

bool Editor::Gui::Widgets::Checkbox( const std::string& str, bool& value ) {
	ImGui::Columns( 2, nullptr, false );

	ImGui::Text( "%s", str.c_str( ) );

	ImGui::NextColumn( );

	bool ret = ImGui::Checkbox( std::string( "##" + str ).c_str( ), &value );

	ImGui::Columns( 1 );

	return ret;
}

bool Editor::Gui::Widgets::Icon( const std::string& text, bool showBackground, Pine::Texture2D* texture, int size, Pine::IAsset* asset ) {
	bool ret = false;

	ImGui::PushID( text.c_str( ) );
	ImGui::BeginGroup( );

	if ( !showBackground ) {
		ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.f, 0.f, 0.f, 0.f ) );
	}

	if ( ImGui::ImageButton( reinterpret_cast< ImTextureID >( texture->GetId( ) ), ImVec2( size, size ), ImVec2( 0.f, 0.f ), ImVec2( 1.f, 1.f ), 3 ) ) {
		ret = true;
	}

	if ( asset != nullptr ) {
		if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_::ImGuiDragDropFlags_None ) ) {
			ImGui::SetDragDropPayload( "Asset", &asset, sizeof( Pine::IAsset* ), 0 );

			ImGui::Image( reinterpret_cast< ImTextureID >( texture->GetId( ) ), ImVec2( 32.f, 32.f ) );

			ImGui::SameLine( );

			ImGui::Text( text.c_str( ) );

			ImGui::EndDragDropSource( );
		}
	}

	if ( !showBackground )
		ImGui::PopStyleColor( );

	ImGui::Text( text.c_str( ) );

	ImGui::EndGroup( );
	ImGui::PopID( );

	return ret;
}

bool Editor::Gui::Widgets::ColorPicker( const std::string& str, glm::vec3& vec ) {
	ImGui::Columns( 2, nullptr, false );

	ImGui::Text( "%s", str.c_str( ) );

	ImGui::NextColumn( );

	ImGui::SetNextItemWidth( -1.f );

	const bool ret = ImGui::ColorEdit3( std::string( "##" + str ).c_str( ), &vec.x );

	ImGui::Columns( 1 );

	return ret;
}
