///
/// Image
///

interface Props {
  image: string; 
};

const Image = ({ image }: Props) => {
  const src = new URL(`../images/${image}`, import.meta.url).href;
  return (
    <div style={{ 
      width: "100%", 
      height: "100%", 
      padding: "20px", 
      display: "flex" 
    }}>
      <img
        src={src}
        style={{
          width: "100%",
          height: "100%",
          objectFit: "cover",
          borderRadius: "12px",
          boxShadow: "0 8px 24px rgba(0, 0, 0, 0.4)",
          display: "block"
        }}
        alt="Macondian Visual"
      />
    </div>
  );
};

export default Image;
