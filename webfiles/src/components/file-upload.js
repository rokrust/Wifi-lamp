import React, {Component} from 'react'

export default class FileUploadButton extends Component {
    constructor(props) {
        super(props);
        this.fileInput = React.createRef();
    }

    render() {
        return (
            <form onSubmit={(e) => {
                e.preventDefault();
                console.log(this.fileInput.current.files[0])
            }}>
                <label>
                    Upload file:
                    <input type="file" ref={this.fileInput} />
                </label>
                <button type="submit">Submit</button>
            </form>
        );
    }
}