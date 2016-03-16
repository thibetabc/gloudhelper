<GameProjectFile>
  <PropertyGroup Type="Layer" Name="no_wifi_layer" ID="11f5a840-99d6-48f0-ad5f-e7521a542e6d" Version="2.0.6.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" FrameEvent="" Tag="40" ctype="LayerObjectData">
        <Position X="0.0000" Y="0.0000" />
        <Scale ScaleX="1.0000" ScaleY="1.0000" />
        <AnchorPoint />
        <CColor A="255" R="255" G="255" B="255" />
        <Size X="1280.0000" Y="720.0000" />
        <PrePosition X="0.0000" Y="0.0000" />
        <PreSize X="0.0000" Y="0.0000" />
        <Children>
          <NodeObjectData Name="bg_layout" ActionTag="-1391830901" Rotation="-90.0000" RotationSkewX="-90.0000" RotationSkewY="-90.0000" Alpha="248" FrameEvent="" Tag="41" ObjectIndex="1" TouchEnable="True" BackColorAlpha="127" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
            <Position X="1280.0000" Y="0.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <AnchorPoint />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="720.0000" Y="1280.0000" />
            <PrePosition X="1.0000" Y="0.0000" />
            <PreSize X="0.5625" Y="1.7778" />
            <Children>
              <NodeObjectData Name="bg_image" ActionTag="-957074508" FrameEvent="" Tag="44" ObjectIndex="1" Scale9Enable="True" LeftEage="7" RightEage="7" TopEage="7" BottomEage="7" Scale9OriginX="7" Scale9OriginY="7" Scale9Width="8" Scale9Height="8" ctype="ImageViewObjectData">
                <Position X="360.0000" Y="640.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="600.0000" Y="420.0000" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.9375" Y="0.4375" />
                <Children>
                  <NodeObjectData Name="title_text" ActionTag="-1741303487" FrameEvent="" Tag="47" ObjectIndex="2" FontSize="36" LabelText="未能连接到Wi-Fi" ctype="TextObjectData">
                    <Position X="300.0000" Y="370.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <Size X="270.0000" Y="36.0000" />
                    <PrePosition X="0.5000" Y="0.8810" />
                    <PreSize X="0.0000" Y="0.0000" />
                  </NodeObjectData>
                  <NodeObjectData Name="line_sprite" ActionTag="1156867492" FrameEvent="" Tag="45" ObjectIndex="2" ctype="SpriteObjectData">
                    <Position X="300.0000" Y="324.0000" />
                    <Scale ScaleX="1.0750" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="558.0000" Y="6.0000" />
                    <PrePosition X="0.5000" Y="0.7714" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="img/green_line.png" />
                  </NodeObjectData>
                  <NodeObjectData Name="info_text" ActionTag="-1181796590" FrameEvent="" Tag="46" ObjectIndex="1" FontSize="36" LabelText="需要连接Wi-Fi，&#xA;在“设备”-“Wi-Fi网络”设置面板选择一个可用的Wi-Fi热点接入。" IsCustomSize="True" HorizontalAlignmentType="HT_Center" ctype="TextObjectData">
                    <Position X="300.0000" Y="200.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <Size X="600.0000" Y="180.0000" />
                    <PrePosition X="0.5000" Y="0.4762" />
                    <PreSize X="1.0000" Y="0.4286" />
                  </NodeObjectData>
                  <NodeObjectData Name="setup_button" ActionTag="500181180" FrameEvent="" Tag="48" ObjectIndex="1" TouchEnable="True" FontSize="36" ButtonText="去设置" Scale9Enable="True" LeftEage="8" RightEage="8" TopEage="8" BottomEage="8" Scale9OriginX="8" Scale9OriginY="8" Scale9Width="8" Scale9Height="5" ctype="ButtonObjectData">
                    <Position X="300.0000" Y="40.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="600.0000" Y="80.0000" />
                    <PrePosition X="0.5000" Y="0.0952" />
                    <PreSize X="1.0000" Y="0.1905" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Normal" Path="img/normal_nowifi_button.9.png" />
                    <PressedFileData Type="Normal" Path="img/select_nowifi_button.9.png" />
                    <NormalFileData Type="Normal" Path="img/normal_nowifi_button.9.png" />
                  </NodeObjectData>
                </Children>
                <FileData Type="Normal" Path="img/white_bg.9.png" />
              </NodeObjectData>
            </Children>
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="0" G="0" B="0" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </NodeObjectData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>